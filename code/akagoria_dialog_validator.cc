/*
 * Akagoria, the revenge of Kalista
 * a single-player RPG in an open world with a top-down view.
 *
 * Copyright (c) 2013-2015, Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cassert>
#include <cstdio>

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>

#include "config.h"

struct DialogData {
  struct Line {
    std::string speaker;
    std::string line;
  };

  std::vector<Line> content;
};

static void loadDialogData(std::map<std::string, DialogData>& dialogues, const std::string& path) {
  try {
    YAML::Node node = YAML::LoadFile(path);

    assert(node.IsMap());

    for (const auto& entry : node) {
      std::string name = entry.first.as<std::string>();
      auto properties = entry.second;

      DialogData data;

      assert(properties.IsMap());

      auto contentNode = properties["content"];

      if (!contentNode) {
        std::fprintf(stderr, "Missing content for entry: '%s'\n", name.c_str());
        continue;
      }

      assert(contentNode.IsSequence());

      for (const auto& itemNode : contentNode) {
        auto speakerNode = itemNode["speaker"];
        assert(speakerNode);
        std::string speaker = speakerNode.as<std::string>();

        auto lineNode = itemNode["line"];
        assert(lineNode);
        std::string line = lineNode.as<std::string>();

        data.content.push_back({ std::move(speaker), std::move(line) });
      }

      dialogues.emplace(std::move(name), std::move(data));
    }
  } catch (std::exception& ex) {
    std::fprintf(stderr, "Error when loading item database: %s\n", ex.what());
    return;
  }
}

static constexpr float SPEAKER_WIDTH = 150.0f;
static constexpr float SPEAKER_HEIGHT = 30.0f;
static constexpr float SPEAKER_PADDING = 5.0f;

static constexpr float DIALOG_WIDTH = 600.0f;
static constexpr float DIALOG_HEIGHT = 90.0f;
static constexpr float DIALOG_BOTTOM = 40.0f;
static constexpr float DIALOG_PADDING = 10.0f;

static constexpr unsigned LINE_SIZE = 20;
static constexpr unsigned SPEAKER_SIZE = 16;

static constexpr float DIALOG_MAX_WIDTH = DIALOG_WIDTH - 2 * DIALOG_PADDING;
static constexpr float DIALOG_MAX_HEIGHT = DIALOG_HEIGHT - 2 * DIALOG_PADDING;

static constexpr float SPEAKER_MAX_WIDTH = SPEAKER_WIDTH - 2 * SPEAKER_PADDING;
static constexpr float SPEAKER_MAX_HEIGHT = SPEAKER_HEIGHT - 2 * SPEAKER_PADDING;

std::string extractBeginning(const std::string& str) {
  static constexpr std::size_t EXTRACT_MAX = 20;

  auto pos = str.find_first_of('\n');

  if (pos == std::string::npos) {
    pos = str.size();
  }

  pos = std::min(pos, EXTRACT_MAX);
  return str.substr(0, pos);
}

int main() {
  boost::filesystem::path basedir = GAME_DATADIR;

  boost::filesystem::path dialogsPath = basedir / "data/dialogues.yml";
  std::map<std::string, DialogData> dialogs;
  loadDialogData(dialogs, dialogsPath.string());

  boost::filesystem::path fontPath = basedir / "fonts/DejaVuSans.ttf";
  sf::Font font;
  font.loadFromFile(fontPath.string());

  for (const auto& item : dialogs) {
    std::printf("Examinining '%s'...\n", item.first.c_str());

    for (const auto& line : item.second.content) {
      sf::Text text;
      text.setFont(font);
      text.setCharacterSize(LINE_SIZE);
      text.setColor(sf::Color::White);

      text.setString(line.line);
      sf::FloatRect rect = text.getLocalBounds();

      float lineWidth = rect.left + rect.width;

      if (lineWidth > DIALOG_MAX_WIDTH) {
        std::fprintf(stderr, "\tLine too wide: %f (max: %f): '%s...'\n", lineWidth, DIALOG_MAX_WIDTH, extractBeginning(line.line).c_str());
      }

      float lineHeight = rect.top + rect.height;

      if (lineHeight > DIALOG_MAX_HEIGHT) {
        std::fprintf(stderr, "\tLine too high: %f (max: %f): '%s...'\n", lineHeight, DIALOG_MAX_HEIGHT, extractBeginning(line.line).c_str());
      }

      text.setCharacterSize(SPEAKER_SIZE);
      text.setString(line.speaker);
      rect = text.getLocalBounds();

      float speakerWidth = rect.left + rect.width;

      if (speakerWidth > SPEAKER_MAX_WIDTH) {
        std::fprintf(stderr, "\tSpeaker too wide: %f (max: %f): '%s'\n", speakerWidth, SPEAKER_MAX_WIDTH, line.speaker.c_str());
      }

      float speakerHeight = rect.top + rect.height;

      if (speakerHeight > SPEAKER_MAX_HEIGHT) {
        std::fprintf(stderr, "\tSpeaker too high: %f (max: %f): '%s'\n", speakerHeight, SPEAKER_MAX_HEIGHT, line.speaker.c_str());
      }

    }
  }

  return 0;
}