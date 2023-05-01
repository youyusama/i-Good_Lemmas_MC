#ifndef VIS_H
#define VIS_H

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "../Model.h"

class Vis {
public:
  Vis();

  ~Vis();

  void addState(std::string s, std::string succ = "");

  /* a gml file sample
Creator "vis"
Version 0.1
graph
[ directed  1
  node
  [ id  0
    graphics
    [ fill "#RRGGBB"
    ]
    LabelGraphics
    [ text  "0"
      color "#RRGGBB"
    ]
  ]
  edge
  [ source  1
    target  0
    graphics
    [ type "arc"
      arrow "last"
    ]
  ]
]
  */
  void OutputGML(bool is_timeout);
  bool isEnoughNodesForVis();

  void add_tree_node(LitVec cube, size_t level);

  void print_tree(size_t k);

private:
  // search vis
  uint tempId;
  std::map<std::string, uint> node_id_map;
  std::vector<std::pair<uint, uint>> m_edges;

  // proof vis
  struct uc_tree_node {
    LitVec cube;
    std::vector<uc_tree_node *> children;
    bool is_same;
    uc_tree_node() {
      cube.clear();
      children.clear();
      is_same = false;
    }
  };
  uc_tree_node *uc_tree_root;

  std::string GetFileName(std::string filePath) {
    auto startIndex = filePath.find_last_of("/");
    if (startIndex == std::string::npos) {
      startIndex = 0;
    } else {
      startIndex++;
    }
    auto endIndex = filePath.find_last_of(".");
    assert(endIndex != std::string::npos);
    return filePath.substr(startIndex, endIndex - startIndex);
  }
};

#endif