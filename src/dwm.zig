const std = @import("std");
const info = @import("info");
const X = @cImport({
    @cInclude("X11/Xlib.h");
});

fn die(writer: anytype, msg: []const u8) noreturn {
    writer.print("{s}\n", .{msg}) catch unreachable;
    std.process.exit(0);
}

fn xerrorstart(_: ?*X.Display, _: [*c]X.XErrorEvent) callconv(.C) c_int {
    die(std.io.getStdErr().writer(), "another window manager is already running");
    return -1;
}

pub fn main() !u8 {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer std.debug.assert(!gpa.deinit());
    const allocator = gpa.allocator();

    const argv = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, argv);
    std.log.debug("{s}", .{argv});

    const stdout = std.io.getStdOut().writer();
    const stderr = std.io.getStdErr().writer();
    if (argv.len == 2 and std.mem.eql(u8, "-v", argv[1])) {
        die(stdout, "dwm-" ++ info.version);
    } else if (argv.len != 1) {
        die(stderr, "usage: dwm [-v]");
    }
    if (X.XOpenDisplay(null)) |dpy| {
        std.log.debug("{}", .{dpy});
        const xerror = X.XSetErrorHandler(xerrorstart);
        _ = xerror;
        X.XSelectInput(
            dpy,
            // X.DefaultRootWindow(dpy),
            // X.ScreenOfDisplay(dpy, X.DefaultScreen(dpy)).*.root,
            &((@ptrCast(?*X._XPrivDisplay, @alignCast(8, dpy))).?.*.*.screens[X.DefaultScreen(dpy)]),
            X.SubstructureRedirectMask,
        );
    } else {
        die(stderr, "failed to open display");
    }

    return 0;
}
