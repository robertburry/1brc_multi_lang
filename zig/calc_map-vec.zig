const std = @import("std");

pub fn main() !void {
    const allocator = std.heap.page_allocator;

    const data_file = "../data/measurements_1_000_000_000.txt";

    // Attempt to open the file, 'file' is directly the file handle if successful
    const file = try std.fs.cwd().openFile(data_file, .{});
    defer file.close(); // Ensure the file is closed when the scope is exited

    // Initialize a buffer for the BufferedReader
    var buffer: [128]u8 = undefined;
    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var city_data = std.StringHashMap(@Vector(4, f16)).init(allocator);
    defer city_data.deinit();

    while (try in_stream.readUntilDelimiterOrEof(&buffer, '\n')) |line_slice| {
        var parts = std.mem.split(u8, line_slice, ";");

        const city: []const u8 = parts.first();
        const temp_str: []const u8 = parts.next().?;

        // std.debug.print("City: {s}\n", .{city});

        const temp: f16 = try std.fmt.parseFloat(f16, temp_str);
        // std.debug.print("Temp: {d}\n", .{temp});

        const city_copy = try allocator.alloc(u8, city.len);
        @memcpy(city_copy, city);
        const stats = try city_data.getOrPut(city_copy);
        if (!stats.found_existing) {
            stats.value_ptr[0] = temp; //Sum
            stats.value_ptr[1] = temp; // Min
            stats.value_ptr[2] = temp; // Max
            stats.value_ptr[3] = 1; // Count

            // try city_data.put(city, stats.value_ptr.*);
        } else {
            stats.value_ptr[0] += temp;
            stats.value_ptr[1] = @min(stats.value_ptr[1], temp);
            stats.value_ptr[2] = @max(stats.value_ptr[2], temp);
            stats.value_ptr[3] += 1;
        }
    }

    var iterator = city_data.iterator();
    const stdout = std.io.getStdOut().writer();
    while (iterator.next()) |entry| {
        const average: f16 = entry.value_ptr[0] / entry.value_ptr[3];
        const city_name = entry.key_ptr;
        // const average: f16 = entry.value_ptr[0];
        // std.debug.print("{s}: {d}/{d}/{d}\n", .{ city_name, entry.value_ptr[1], average, entry.value_ptr[2] });
        try stdout.print("{s}: ", .{city_name.*});
        try stdout.print("{d}/", .{entry.value_ptr[0]});
        try stdout.print("{d}/", .{average});
        try stdout.print("{d} \n", .{entry.value_ptr[2]});
    }
}
