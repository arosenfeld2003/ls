require 'open3'
require 'colorize'
require 'tempfile'
require 'tmpdir'

def my_exec(cmds)
    stdout_str, error_str, status = Open3.capture3(*cmds)
end

def formattage_output(result)
    result.gsub("\e[0m", '').gsub("\e[1;34m", '').gsub(/\s+/m, ' ').strip.gsub("\n", '')
end

def run(*options)
    puts "RUN with: |#{options.join(' ')}|"
    result, stderr, status  = my_exec(["ls", "-x"] + options)
    result2, _, _ = my_exec(["./my_ls"] + options)

    if formattage_output(result) == formattage_output(result2)
        puts "Compare => #{"true".light_green}"
        puts "my_ls\n#{formattage_output(result2)}"
    else
        puts "Compare => #{"false".light_red}"
        puts "ls\n|#{formattage_output(result)}|"
        puts "my_ls\n|#{formattage_output(result2)}|"
    end
end

run()
run("-a")
run("/tmp/")
run("i_dont_exist")

Dir.mktmpdir do |dir|
    Tempfile.new('spider-man', [tmpdir = dir])
    Tempfile.new('iron-man', [tmpdir = dir])
    Tempfile.new('black panther', [tmpdir = dir])
    Tempfile.new('dead pool', [tmpdir = dir])
    Tempfile.new('captain america', [tmpdir = dir])
    Tempfile.new('ant-man', [tmpdir = dir])
    Tempfile.new('captain marvel', [tmpdir = dir])
    Tempfile.new('wolverine', [tmpdir = dir])

    run("/tmp/", dir)
    run("-t", dir)
    run("-ta", dir)
    run("-t", "-a", dir)
end