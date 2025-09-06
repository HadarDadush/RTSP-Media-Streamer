# RTSP Media Streamer

## Overview
This project demonstrates real-time media streaming in C using GStreamer.  
It includes an RTSP server that streams video and audio, and a client application with playback controls such as play, pause, seek forward/backward, and quit.

## Features
- RTSP server that streams a local video file with audio.
- Client application that connects to the server and plays the stream.
- Interactive playback controls:
  - Play/Pause
  - Seek forward (+10s) and backward (-10s)
  - Quit gracefully
- Handles end-of-stream and error events.

## Technologies Used
- C (ANSI C)
- GStreamer (core library and RTSP server module)

## Requirements
- GStreamer installed (including `gst-rtsp-server` plugin).
- A local video file (update the file path in `server.c`).
- Compiler for C (e.g., GCC or Visual Studio).

## Usage
1. Build the project:
   ```bash
   gcc main.c server.c client.c -o RTSP-Streamer `pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0`
2. Run the server:
  ./RTSP-Streamer server
  The server will start and stream your configured video at: rtsp://127.0.0.1:8555/test
3. Run the client:
  ./RTSP-Streamer client
4. Use keyboard controls to interact:
   P = Play/Pause
   F = Seek forward 10s
   B = Seek backward 10s
   Q = Quit
   
