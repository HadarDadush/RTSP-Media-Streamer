// client.c — RTSP client with keyboard controls (Windows)
// Keys: P = Pause/Play, F = +10s, B = -10s, Q = Quit

#include <gst/gst.h>
#include <windows.h>   // For Sleep
#include <conio.h>     // For keyboard input

// Function to move video forward/backward
static gboolean seek_by(GstElement* pipeline, gint64 delta_ns) {
    gint64 pos = 0;
    if (!gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)) return FALSE;
    gint64 target = pos + delta_ns;
    if (target < 0) target = 0;

    // Jump to new time in the video
    return gst_element_seek_simple(
        pipeline, GST_FORMAT_TIME,
        (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),
        target);
}

int run_client(void) {
    gst_init(NULL, NULL); // Start GStreamer

    const char* URL = "rtsp://127.0.0.1:8555/test"; // Video link
    GstElement* pipeline = gst_element_factory_make("playbin", NULL); // Create player
    if (!pipeline) { g_printerr("Failed to create playbin\n"); return 1; }

    g_object_set(pipeline, "uri", URL, NULL); // Set the link
    gst_element_set_state(pipeline, GST_STATE_PLAYING); // Start playing
    g_print("RTSP client playing: %s\n", URL);
    g_print("Controls: [P]=Pause/Play  [F]=+10s  [B]=-10s  [Q]=Quit\n");

    GstBus* bus = gst_element_get_bus(pipeline); // For error and end messages
    gboolean running = TRUE;
    gboolean paused = FALSE;

    while (running) {

        // Check for errors or end of video
        while (TRUE) {
            GstMessage* msg = gst_bus_pop_filtered(
                bus, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
            if (!msg) break;

            switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR: {

                // Print error
                GError* err = NULL; gchar* dbg = NULL;
                gst_message_parse_error(msg, &err, &dbg);
                g_printerr("ERROR: %s\n", err ? err->message : "unknown");
                if (dbg) g_printerr("Debug: %s\n", dbg);
                if (err) g_error_free(err); g_free(dbg);
                running = FALSE;
                break;
            }
            case GST_MESSAGE_EOS:

                // Video finished
                g_print("End of stream\n");
                running = FALSE;
                break;
            default: break;
            }
            gst_message_unref(msg); // Free message
        }

        // Check keyboard keys
        if (_kbhit()) {
            int c = _getch();
            if (c == 'q' || c == 'Q') {
                running = FALSE; // Quit
            }
            else if (c == 'p' || c == 'P') {
                // Pause or Play
                paused = !paused;
                gst_element_set_state(pipeline, paused ? GST_STATE_PAUSED : GST_STATE_PLAYING);
                g_print(paused ? "Paused\n" : "Playing\n");
            }
            else if (c == 'f' || c == 'F') {
                // Forward 10s
                if (seek_by(pipeline, 10 * GST_SECOND))
                    g_print("Seek +10s\n");
            }
            else if (c == 'b' || c == 'B') {
                // Backward 10s
                if (seek_by(pipeline, -10 * GST_SECOND))
                    g_print("Seek -10s\n");
            }
        }

        Sleep(10); // Small wait
    }

    // Close and clean
    gst_element_set_state(pipeline, GST_STATE_NULL); // Stop
    gst_object_unref(bus);
    gst_object_unref(pipeline);
    return 0;
}
