// server.c — RTSP server function (no main)
// Video H.264 + Audio Opus

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

int run_server(void) {
    // Initialize GStreamer
    gst_init(NULL, NULL);

    // Create RTSP server on TCP port 8555
    GstRTSPServer* server = gst_rtsp_server_new();
    gst_rtsp_server_set_service(server, "8555");

    // Create mount points and media factory
    GstRTSPMountPoints* mounts = gst_rtsp_server_get_mount_points(server);
    GstRTSPMediaFactory* factory = gst_rtsp_media_factory_new();

    // Set your video file path
    const gchar* video_path = "C:/Users/hadar/video.mp4";

    // Pipeline (video + audio):
    //   filesrc -> decodebin ->
    //     video: videoconvert -> x264enc -> rtph264pay (pay0, pt=96)
    //     audio: audioconvert -> audioresample -> opusenc -> rtpopuspay (pay1, pt=97)
    gchar* launch = g_strdup_printf(
        "( filesrc location=\"%s\" ! decodebin name=d "
        "  d. ! queue ! videoconvert ! "
        "      x264enc tune=zerolatency speed-preset=veryfast bitrate=2000 key-int-max=60 ! "
        "      rtph264pay name=pay0 pt=96 config-interval=1 "
        "  d. ! queue ! audioconvert ! audioresample ! "
        "      opusenc ! rtpopuspay name=pay1 pt=97 )",
        video_path);


    gst_rtsp_media_factory_set_launch(factory, launch);
    g_free(launch);

    // Share one pipeline between multiple clients
    gst_rtsp_media_factory_set_shared(factory, FALSE);

    // Expose stream at rtsp://127.0.0.1:8555/test
    gst_rtsp_mount_points_add_factory(mounts, "/test", factory);
    g_object_unref(mounts);

    // Start server
    if (!gst_rtsp_server_attach(server, NULL)) {
        g_printerr("Failed to attach RTSP server\n");
        g_object_unref(server);
        return 1;
    }

    g_print("RTSP server ready: rtsp://127.0.0.1:8555/test\n");
    g_print("Press Ctrl+C to stop.\n");

    // Run main loop
    GMainLoop* loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    // Cleanup
    g_main_loop_unref(loop);
    g_object_unref(server);
    return 0;
}
