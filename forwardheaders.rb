#!/usr/bin/env ruby

# this script is responsible for generating header forwardings
# directly in Sources/API directory to allow library user to do
# something like that:
# #include <ClanLib/DisplayWindow>
# instead of
# #include <ClanLib/core.h>

API_DIR = 'Sources/API/' # correct: 'dir1/dir2/', incorrect: 'dir1/dir2'

class ClassEntry
    attr_accessor :class_name
    attr_accessor :location
    
    def to_s
        return "#<class_name=#{class_name}, location=#{location}>"
    end
end

def scan_header(header_path)
    puts "scanning header #{header_path}"
    class_entries = []

    File.open(header_path, "r") do |file|
        file.each_line do |line|
            if line =~ /^class (CL_API_[a-zA-Z0-9_]+ )?CL_([a-zA-Z0-9_;]+)/
            
                if $2[-1..-1] == ';'
                    next
                end
            
                class_entry = ClassEntry.new
                class_entry.class_name = $2
                class_entry.location = header_path
                
                puts "found class #{class_entry}"
                
                class_entries.push class_entry
            end
        end
    end
    
    if class_entries.empty?
        puts "not found any class in #{header_path}"
    end
    
    return class_entries
end

def scan(path)

    puts "scanning #{path}..."

    result = []

    Dir.open(path) do |dir|
        dir.each do |entry|
            if entry == '.' or entry == '..' or entry == ".svn"
                next
            end
            
            full_path = path + '/' + entry
            scan_result = nil
            
            if File.file? full_path
                scan_result = scan_header(full_path)
            else
                scan_result = scan(full_path)
            end
            
            result = result + scan_result
        end
    end
    
    return result
end

def generate_forward(class_entry)
    path = API_DIR + class_entry.class_name + ".h"
    
    if not File.exists? path
        puts "writing file #{path}"
        File.open(path , "w") do |file|
            file << "#pragma once\n"
            
            relpath = class_entry.location[(API_DIR.size + 1)..-1]
            file << "#include \"#{relpath}\""
        end
    else
        puts "ERROR: file #{path} exists"
        exit 1
    end
end

def generate_forwards(class_entries)
    class_entries.each do |entry|
        generate_forward(entry)
    end
end

entries = scan(API_DIR)
generate_forwards(entries)
