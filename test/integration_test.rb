require File.expand_path(File.dirname(__FILE__) + "/test_helper.rb")

class IntegrationTest < Test::Unit::TestCase
  def setup
    prepare_input([
      ["e1", "e2"],
      ["e1", "e3"],
      ["e1", "e4"],
      ["e1", "e4"],
      ["e2", "e3"],
      ["e2", "e4"],
      ["e3", "e4"]
    ])
  end

  def test_http_response_with_center_e1
    assert_equal([
      ["e1", "e2"],
      ["e1", "e3"],
      ["e1", "e4"],
      ["e1", "e4"]
    ].sort, curl(:center => "e1", :depth => 1).sort)

    assert_equal([
      ["e1", "e2"],
      ["e1", "e3"],
      ["e1", "e4"],
      ["e1", "e4"],

      ["e2", "e3"],
      ["e2", "e4"],

      ["e3", "e4"]
    ].sort, curl(:center => "e1", :depth => 2).sort)

    assert_equal([
      ["e1", "e2"],
      ["e1", "e3"],
      ["e1", "e4"],
      ["e1", "e4"],

      ["e2", "e3"],
      ["e2", "e4"],

      ["e3", "e4"]
    ].sort, curl(:center => "e1", :depth => 3).sort)
  end

  def test_http_response_with_center_e2
    assert_equal([
      ["e2", "e3"],
      ["e2", "e4"]
    ].sort, curl(:center => "e2", :depth => 1).sort)

    assert_equal([
      ["e2", "e3"],
      ["e2", "e4"],

      ["e3", "e4"]
    ].sort, curl(:center => "e2", :depth => 2).sort)
  end

  def test_http_response_with_center_e2
    assert_equal([
      ["e3", "e4"]
    ].sort, curl(:center => "e3", :depth => 1).sort)
  end
end
