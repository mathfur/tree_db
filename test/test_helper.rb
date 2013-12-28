require "test/unit"
require "benchmark"

BASE_DIR = File.dirname(__FILE__) + "/.."
TMP_DIR = "#{BASE_DIR}/tmp"

class Test::Unit::TestCase
  def prepare_input(pairs)
    open("#{TMP_DIR}/data.txt", "w") do |f|
      pairs.each do |k, v|
        f.write "#{k},#{v}"
      end
    end
  end

  def curl(opt={})
    result = []

    IO.popen("#{BASE_DIR}/tree_db", "r+") do |io|
      url = "http://localhost:12355?#{opt.map{|k,v| "#{k}=#{v}"}.join('&')}"
      `curl -s '#{url}'`.scan(/(\w+),(\w+)/).each do |k, v|
        result << [k, v]
      end
    end

    return result
  end
end
