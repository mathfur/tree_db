num = 1
threshold = 10
10.times do
  t = `time build/test_performance NUM=#{num}`[/ ([0-9\.]+) total/, 1]

  puts "#{t}"

  if(threshold < t.to_f)
    break
  end

  num *= 2
end
