require File.expand_path(File.dirname(__FILE__) + "/test_helper.rb")

class PerformanceTest < Test::Unit::TestCase
  def test_1
    n = 1
    max = 10

    100.times do |i|
      hash = {}
      n.times do |j|
        hash["e#{j}"] = "e#{j+1}"
      end
      prepare_input(hash)

      t = (Benchmark.measure { curl(:center => "e1", :depth => n+1) }).real
      puts "n:#{n}, t:#{t}"

      if(max < t.to_f)
        break
      end

      n *= 2
    end
  end
end
