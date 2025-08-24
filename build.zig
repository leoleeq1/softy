const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});
    // const mode: std.builtin.OptimizeMode = .Debug;

    const flags: [18][]const u8 = .{
        "-std=c++23",
        "-Wall",
        "-Wextra",
        "-Wshadow",
        "-Wnon-virtual-dtor",
        "-pedantic",
        "-Wno-gnu-anonymous-struct",
        "-Wno-nested-anon-types",
        "-Wformat",
        "-Wformat=2",
        "-Wconversion",
        "-Wimplicit-fallthrough",
        "-Werror=format-security",
        "-D_FORTIFY_SOURCE=3",
        "-fstrict-flex-arrays=3",
        "-fstack-clash-protection",
        "-fstack-protector-strong",
        "-fPIE",
    };
    const exe = b.addExecutable(.{
        .name = "softy",
        .target = target,
        .optimize = mode,
    });
    exe.addCSourceFiles(.{
        .files = &.{
            "src/main.cpp",
            "src/core/transform.cpp",
            "src/render/buffer.cpp",
            "src/render/camera.cpp",
            "src/render/forward_render_pipeline.cpp",
            "src/render/mesh.cpp",
            "src/render/rasterizer.cpp",
            "src/shader/shader.cpp",
        },
        .flags = &flags,
        .language = .cpp,
    });
    exe.addIncludePath(b.path("src"));

    switch (target.result.os.tag) {
        .windows => {
            exe.addCSourceFiles(.{
                .files = &.{
                    "src/window/window_win32.cpp",
                },
                .flags = &flags,
                .language = .cpp,
            });
            exe.linkSystemLibrary("gdi32");
        },
        .linux => {},
        .macos => {},
        else => |os_tag| std.debug.print("Unsupported OS: {s}", .{@tagName(os_tag)}),
    }
    exe.linkLibCpp();

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const tester = b.addExecutable(.{
        .name = "tester",
        .target = target,
        .optimize = mode,
    });
    tester.addCSourceFiles(.{
        .files = &.{
            "tests/tester.cpp",
        },
        .flags = &flags,
        .language = .cpp,
    });
    tester.addIncludePath(b.path("src"));
    tester.addIncludePath(b.path("tests"));

    switch (target.result.os.tag) {
        .windows => {},
        .linux => {},
        .macos => {},
        else => |os_tag| std.debug.print("Unsupported OS: {s}", .{@tagName(os_tag)}),
    }
    tester.linkLibCpp();

    b.installArtifact(tester);

    const tester_run_cmd = b.addRunArtifact(tester);
    tester_run_cmd.step.dependOn(b.getInstallStep());

    const tester_run_step = b.step("test", "Test the app");
    tester_run_step.dependOn(&tester_run_cmd.step);
}
