require "test/unit"
require "benchmark"

BASE_DIR = File.dirname(__FILE__) + "/.."
TMP_DIR = "#{BASE_DIR}/tmp"

PORT_NUM = 12355
DATA_FNAME = "#{TMP_DIR}/testdata.txt"

class Test::Unit::TestCase
  def prepare_input(pairs)
    open(DATA_FNAME, "w") do |f|
      pairs.each do |k, v|
        f.puts "#{k},#{v}"
      end
    end
  end

  def curl(opt={})
    result = []

    IO.popen("#{BASE_DIR}/tree_db -p #{PORT_NUM} -s #{DATA_FNAME}", "r+") do |io|
      url = "http://127.0.0.1:#{PORT_NUM}?#{opt.map{|k,v| "#{k}=#{v}"}.join('&')}"

      `curl -s '#{url}'`.scan(/(\w+),(\w+)/).each do |k, v|
        result << [k, v]
      end
    end

    return result
  end
end
