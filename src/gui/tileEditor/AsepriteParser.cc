
#include "AsepriteParser.hh"
#include "nlohmann/json.hpp"
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>

AsepriteParser::AsepriteParser(LabelSplitter_t splitter)
  : label_parser_(splitter)
{}

AsepriteParser::LabelSplit AsepriteParser::defaultSplitter(std::string const& raw) {
  std::stringstream ss(raw);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(ss, line, '-'))
  {
    lines.emplace_back(std::move(line));
  }

  if (lines.size() < 2)
  {
    throw std::out_of_range("Not enough elements in label");
  }

  LabelSplit split;
  if (lines.size() >= 3)
  {
    split.frame_label = lines[1];
    split.index = std::stoul(lines[2]);
  } else {
    split.frame_label = lines[0];
    split.index = std::stoul(lines[1]);
  }
  return split;
}

bool AsepriteParser::isValidRecord(nlohmann::json const &r) const {
  if (!r.contains("frame") ||
      !r.contains("rotated") ||
      !r.contains("trimmed")) {
    return false;
  }

  if (!r["rotated"].is_boolean() || !r["trimmed"].is_boolean()) {
    return false;
  }

  if (!r["frame"].is_object()) {
    return false;
  }
  nlohmann::json const& frame = r["frame"];
  if (!frame.contains("x") || !frame["x"].is_number_unsigned() ||
      !frame.contains("y") || !frame["y"].is_number_unsigned() ||
      !frame.contains("w") || !frame["w"].is_number_unsigned() ||
      !frame.contains("h") || !frame["h"].is_number_unsigned()) {
    return false;
  }

  return true;
}

std::variant<TileSet, ITileParser::TileParseFault> AsepriteParser::parse(std::string const &filename) {
  std::ifstream input(filename);

  nlohmann::json json_parsed = nlohmann::json::parse(input, nullptr, false);

  if (json_parsed.is_discarded())
  {
    return TileParseFault {
      "Could not parse JSON",
      0,
      0
    };
  }

  if (!json_parsed.contains("meta"))
  {
    return TileParseFault {
      "Does not contain 'meta' field",
      0,
      0
    };
  }

  if (!json_parsed.contains("frames"))
  {
    return TileParseFault {
      "Does not contain 'frames' field",
      0,
      0
    };
  }

  nlohmann::json &frames = json_parsed["frames"];
  nlohmann::json &meta = json_parsed["meta"];

  if (!meta.contains("image"))
  {
    return TileParseFault {
      "Does not contain 'meta'.'image' field",
      0,
      0
    };
  }

  if (!meta.contains("size"))
  {
    return TileParseFault {
      "Does not contain 'meta'.'size' field",
      0,
      0
    };
  }

  nlohmann::json &size = meta["size"];

  if (!size.contains("h") || !size.contains("w"))
  {
    return TileParseFault {
      "'meta'.'size' field does not contain 'w' and 'h'",
      0,
      0
    };
  }

  if (!size["h"].is_number_unsigned() || !size["w"].is_number_unsigned())
  {
    return TileParseFault {
      "'meta'.'size'.['w','h'] is not a unsigned type",
      0,
      0
    };
  }

  if (!meta["image"].is_string())
  {
    return TileParseFault {
      "'meta'.'image' is not a string type",
      0,
      0
    };
  }

  if (!frames.is_object()) {
    return TileParseFault {
      "'frames' is not a object type",
      0,
      0
    };
  }

  TileSet result;
  result.image_name = meta["image"];

  auto path_to_image = std::filesystem::path(filename);
  path_to_image.replace_filename(result.image_name);

  result.image_path = path_to_image.string();
  result.width = size["w"];
  result.height = size["h"];

  for (auto const &record : frames.items()) {
    auto const& frame = record.value();
    std::string const& label = record.key();

    if (!isValidRecord(frame)) {
      continue;
    }

    auto labelSplit = label_parser_(label);

    TileSet::TileFrame tileFrame;

    tileFrame.index = labelSplit.index;

    auto &frameDimensions = frame["frame"];

    tileFrame.x = frameDimensions["x"];
    tileFrame.y = frameDimensions["y"];
    tileFrame.width = frameDimensions["w"];
    tileFrame.height = frameDimensions["h"];

    result.frames.emplace_back(tileFrame);
  }

  std::sort(result.frames.begin(), result.frames.end(),
    [](TileSet::TileFrame const&a, TileSet::TileFrame const&b) {
      return a.index < b.index;
    });

  return result;
}

const char *AsepriteParser::getFileExtensions() const {
  return "json";
}