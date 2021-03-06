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
#include <boost/locale.hpp>

#include <SFML/Graphics.hpp>

#include <yaml-cpp/yaml.h>

#include "config.h"

struct DialogData {
  struct Line {
    std::string speaker;
    std::string words;
  };

  std::vector<Line> content;
};

static sf::String convertString(const std::string& str) {
  std::basic_string<sf::Uint32> u32String;
  u32String.reserve(str.size());
  sf::Utf8::toUtf32(str.begin(), str.end(), std::back_inserter(u32String));
  return sf::String(u32String);
}

static sf::String convertLocalString(const std::string& str) {
  std::string translatedString = boost::locale::gettext(str.c_str());
  return convertString(translatedString);
}

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

      for (const auto& lineNode : contentNode) {
        auto speakerNode = lineNode["speaker"];
        assert(speakerNode);
        auto speaker = convertString(speakerNode.as<std::string>());

        auto wordsNode = lineNode["words"];
        assert(wordsNode);
        auto words = convertLocalString(wordsNode.as<std::string>());

        data.content.push_back({ std::move(speaker), std::move(words) });
      }

      dialogues.emplace(std::move(name), std::move(data));
    }
  } catch (std::exception& ex) {
    std::fprintf(stderr, "Error when loading item database: %s\n", ex.what());
    return;
  }
}

static constexpr unsigned SPEAKER_SIZE = 16;
static constexpr float SPEAKER_WIDTH = 150.0f;
static constexpr float SPEAKER_HEIGHT = 25.0f;
static constexpr float SPEAKER_PADDING = 5.0f;

static constexpr unsigned WORDS_SIZE = 20;
static constexpr float WORDS_WIDTH = 600.0f;
static constexpr float WORDS_HEIGHT = 90.0f;
static constexpr float WORDS_PADDING = 10.0f;

static constexpr float WORDS_MAX_WIDTH = WORDS_WIDTH - 2 * WORDS_PADDING;
static constexpr float WORDS_MAX_HEIGHT = WORDS_HEIGHT - 2 * WORDS_PADDING;

static constexpr float SPEAKER_MAX_WIDTH = SPEAKER_WIDTH - 2 * SPEAKER_PADDING;
static constexpr float SPEAKER_MAX_HEIGHT = SPEAKER_HEIGHT - 2 * SPEAKER_PADDING;

std::string extractBeginning(const sf::String& str) {
  static constexpr std::size_t EXTRACT_MAX = 20;

  auto end = std::find(str.begin(), str.end(), sf::Uint32('\n'));
  std::size_t size = end - str.begin();
  size = std::min(size, EXTRACT_MAX);

  std::string u8str;

  for (std::size_t i = 0; i < size; ++i) {
    sf::Utf32::toUtf8(str.getData() + i, str.getData() + i + 1, std::back_inserter(u8str));
  }

  return u8str;
}

int main() {
  boost::locale::generator localeGenerator;
  localeGenerator.add_messages_path(GAME_LOCALEDIR);
  localeGenerator.add_messages_domain("akagoria");
  std::locale::global(localeGenerator(""));

  std::printf("Locale is: %s\n", std::locale("").name().c_str());

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
      text.setCharacterSize(WORDS_SIZE);
      text.setColor(sf::Color::White);

      text.setString(line.words);
      sf::FloatRect rect = text.getLocalBounds();

      float wordsWidth = rect.width;

      if (wordsWidth > WORDS_MAX_WIDTH) {
        std::fprintf(stderr, "\tLine too wide: %f (max: %f): '%s...'\n", wordsWidth, WORDS_MAX_WIDTH, extractBeginning(line.words).c_str());
      }

      float wordsHeight = rect.height;

      if (wordsHeight > WORDS_MAX_HEIGHT) {
        std::fprintf(stderr, "\tLine too high: %f (max: %f): '%s...'\n", wordsHeight, WORDS_MAX_HEIGHT, extractBeginning(line.words).c_str());
      }

      text.setCharacterSize(SPEAKER_SIZE);
      text.setString(line.speaker);
      rect = text.getLocalBounds();

      float speakerWidth = rect.width;

      if (speakerWidth > SPEAKER_MAX_WIDTH) {
        std::fprintf(stderr, "\tSpeaker too wide: %f (max: %f): '%s'\n", speakerWidth, SPEAKER_MAX_WIDTH, line.speaker.c_str());
      }

      float speakerHeight = rect.height;

      if (speakerHeight > SPEAKER_MAX_HEIGHT) {
        std::fprintf(stderr, "\tSpeaker too high: %f (max: %f): '%s'\n", speakerHeight, SPEAKER_MAX_HEIGHT, line.speaker.c_str());
      }

    }
  }

  return 0;
}
