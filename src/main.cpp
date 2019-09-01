// Copyright 2018 Linus Klöckner

#ifndef ANDROID

#include <boost/program_options.hpp>
#include <string>
#include <vector>

#include "YoutubeListDownloader/download/options.h"
#include "YoutubeListDownloader/logger.h"
#include "YoutubeListDownloader/youtube/playlist.h"

namespace po = boost::program_options;

int main(int p_argument_count, char** p_argument_vector) {
    try {
        po::options_description desc{"Options"};
        // TODO: option for custom folder
        desc.add_options()("help,h", "Print help message")("video-title,t", "Save file with video title as filename")("download-video,v", po::value<std::string>(), "Download the video with given ID")("download-channel,c", po::value<std::string>(), "Download the whole channel with given ID")("download-user,u", po::value<std::string>(), "Download the whole channel with given users ID")("download-playlist,p", po::value<std::string>(), "Download the whole playlist with given ID")("api-key,k", po::value<std::string>(), "The API-Key to be used for communication with the youtube-api")("itag,q", po::value<int>(), "Format itag to download")("stub,s", "For testing..")("available-itags,i", po::value<std::string>(), "Shows all available itags for given video ID");

        po::variables_map vm;

        po::store(po::parse_command_line(p_argument_count, p_argument_vector, desc), vm);

        if (vm.count("api-key") != 0u) {
            Youtube::Settings::Instance().SetCustomApiKey(vm["api-key"].as<std::string>());
        }

        if (vm.count("help") != 0u) {
            std::cout << "Basic Command Line Parameter App" << std::endl
                      << desc << std::endl;
            return 0;
        }

        Download::Options& download_options = Download::Options::GlobalOptions();

        if (vm.count("itag") != 0u){
            download_options.m_itags = {};
            download_options.m_itags.push_back(vm["itag"].as<int>());
        }

        if (vm.count("itag") != 0u){
            download_options.m_save_video_name = true;
        }

        if (vm.count("available-itags") != 0u) {
            Youtube::Video video = Youtube::Video::Get(vm["available-itags"].as<std::string>());
            video.LoadDownloadLinks();
            video.PrintFormats();
            return 0;
        }

        if (vm.count("download-channel") != 0u) {
            Youtube::Channel channel = Youtube::Channel::Get(vm["download-channel"].as<std::string>(), false);
            channel.LoadVideos()->join();
            channel.DownloadVideos();
            return 0;
        }

        if (vm.count("download-user") != 0u) {
            Youtube::Channel channel = Youtube::Channel::Get(vm["download-user"].as<std::string>(), true);
            channel.LoadVideos()->join();
            channel.DownloadVideos();
            return 0;
        }

        if (vm.count("download-playlist") != 0u) {
            Youtube::Playlist playlist = Youtube::Playlist::Get(vm["download-playlist"].as<std::string>());
            playlist.LoadVideos()->join();
            playlist.DownloadVideos();
            return 0;
        }

        if (vm.count("download-video") != 0u) {
            Youtube::Video video = Youtube::Video::Get(vm["download-video"].as<std::string>());
            video.LoadDownloadLinks();
            Youtube::Video::Download(video);
            return 0;
        }

        if (vm.count("stub") != 0u) {
            // Only for testing
        }
    } catch (std::exception& e) {
        LogException(e);
    }

    return 0;
}
#endif
