const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{ .preferred_optimize_mode = .ReleaseFast });

    const exe = b.addExecutable(.{
        .name = "softy",
        .target = target,
        .optimize = mode,
    });
    exe.addCSourceFiles(.{ .files = &.{
        "src/main.cpp",
    }, .flags = &.{
        "-std=c++23",
        "-Wall",
        "-Wextra",
        "-Wshadow",
        "-Wnon-virtual-dtor",
        "-pedantic",
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
    } });
    exe.linkLibCpp();

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
