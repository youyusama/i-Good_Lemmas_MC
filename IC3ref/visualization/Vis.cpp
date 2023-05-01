#include "Vis.h"
#include <stdio.h>

Vis::Vis() {
  node_id_map.clear();
  m_edges.clear();
  tempId = 0;
  uc_tree_root = new uc_tree_node();
}

Vis::~Vis() {
  node_id_map.clear();
  m_edges.clear();
  tempId = 0;
}

void latches_vecotr_to_short_vector(std::vector<uint32_t> &node, std::vector<int> &latches) {
  int count = 0;
  uint32_t tempi = 0;
  for (int l : latches) {
    if (l > 0)
      tempi = (tempi << 1) + 1;
    else
      tempi <<= 1;
    count++;
    if (count == 32 || l == latches[latches.size() - 1]) {
      node.emplace_back(tempi);
      tempi = 0;
      count = 0;
    }
  }
}

void Vis::addState(std::string s, std::string succ) {
  uint temp_node_id = tempId++;
  // add node
  auto z = node_id_map.insert(std::pair<std::string, uint>(s, temp_node_id));
  if (z.second == false) { // visit the same state repeatedly
    // std::cout<<"vis repeat"<<std::endl;
    tempId--;
    // std::cout<<z.first->second<<std::endl;
    // for (int i=0; i<z.first->first.size(); i++) std::cout<<z.first->first[i];
    // std::cout<<std::endl;
    // for (int i=0; i<temp_node.size(); i++) std::cout<<temp_node[i];
    // std::cout<<std::endl;
    std::map<std::string, uint>::iterator iter;
    iter = node_id_map.find(s);
    if (iter != node_id_map.end()) {
      temp_node_id = iter->second;
    } else {
      // place holder
    }
  }

  // add edge
  if (succ != "") {
    // find prenode
    std::map<std::string, uint>::iterator iter;
    iter = node_id_map.find(succ);
    if (iter != node_id_map.end()) {
      m_edges.push_back(std::pair<uint, uint>(iter->second, temp_node_id));
    } else {
      // place holder
    }
  }
}

std::string to_hex(int i) {
  std::stringstream ioss;
  std::string s;
  ioss << std::hex << i;
  ioss >> s;
  if (s.size() == 1) {
    s = "0" + s;
  } else if (s.size() > 2) {
    // place holder
  }
  return s;
}

std::string Vis_GetColor(int step, int n) {
  if (n == step - 1) {
    return "\"#DC143C\"";
  }
  // set gradual color
  // terminal 0,0,0 -> 15,155,15
  float r_a = 0, g_a = 0, b_a = 0;
  float r_b = 15, g_b = 155, b_b = 15;
  int r_n, g_n, b_n;
  r_n = (int)r_a + (r_b - r_a) / step * n;
  g_n = (int)g_a + (g_b - g_a) / step * n;
  b_n = (int)b_a + (b_b - b_a) / step * n;
  std::string res = "";
  res += to_hex(r_n);
  res += to_hex(g_n);
  res += to_hex(b_n);
  return "\"#" + res + "\"";
}

void write_node(std::ofstream &visFile, uint id, std::string text = "", std::string color = "\"#008000\"") {
  text = (text.length() == 0) ? std::to_string(id) : text;
  visFile << "node" << std::endl;
  visFile << "[" << std::endl;
  visFile << "id\t" << id << std::endl;
  visFile << "graphics" << std::endl;
  visFile << "[" << std::endl;
  visFile << "w\t" << 50 << std::endl;
  visFile << "h\t" << 160 << std::endl;
  visFile << "fill\t" << color << std::endl;
  visFile << "]" << std::endl;
  visFile << "LabelGraphics" << std::endl;
  visFile << "[" << std::endl;
  visFile << "text\t"
          << "\"" << text << "\"" << std::endl;
  visFile << "color\t"
          << "\"#333333\"" << std::endl;
  visFile << "]" << std::endl;
  visFile << "]" << std::endl;
}

void write_edge(std::ofstream &visFile, uint source, uint target) {
  visFile << "edge" << std::endl;
  visFile << "[" << std::endl;
  visFile << "source\t" << source << std::endl;
  visFile << "target\t" << target << std::endl;
  visFile << "graphics" << std::endl;
  visFile << "[" << std::endl;
  visFile << "type\t"
          << "\"arc\"" << std::endl;
  visFile << "arrow\t"
          << "\"last\"" << std::endl;
  visFile << "]" << std::endl;
  visFile << "]" << std::endl;
}

void Vis::OutputGML(bool is_timeout) {
  // io
  std::ofstream visFile;
  visFile.open("vis.gml");
  // header
  if (is_timeout)
    visFile << "Creator\t"
            << "\"car visualization (time out)\"" << std::endl;
  else
    visFile << "Creator\t"
            << "\"car visualization\"" << std::endl;
  visFile << "Version\t" << 0.1 << std::endl;
  visFile << "graph" << std::endl;
  visFile << "[" << std::endl;
  // graph info
  visFile << "directed\t" << 1 << std::endl;
  // search nodes
  std::map<std::string, uint>::iterator iter;
  for (iter = node_id_map.begin(); iter != node_id_map.end(); iter++) {
    write_node(visFile, iter->second, "", Vis_GetColor(tempId, iter->second));
  }
  // edges
  for (auto e : m_edges) {
    write_edge(visFile, e.first, e.second);
  }
  visFile << "]" << std::endl;
  visFile.close();
}

bool Vis::isEnoughNodesForVis() {
  if (node_id_map.size() >= 10000)
    return true;
  else
    return false;
}

bool _LitComp(const Minisat::Lit &l1, const Minisat::Lit &l2) {
  return l1 < l2;
}

// ================================================================================
// @brief: whether cube1 -> cube2
// @input:
// @output:
// ================================================================================
bool _cube_imply(LitVec cube1, LitVec cube2) {
  if (std::includes(cube2.begin(), cube2.end(), cube1.begin(), cube1.end(), _LitComp))
    return true;
  else
    return false;
}

// ================================================================================
// @brief: whether cube1 == cube2
// @input:
// @output:
// ================================================================================
bool _cube_same(LitVec cube1, LitVec cube2) {
  if (cube1.size() != cube2.size()) return false;

  for (int i = 0; i < cube1.size(); i++) {
    if (cube1[i] != cube2[i]) return false;
  }
  return true;
}

// ================================================================================
// @brief: add node to the frame tree
// @input: the cube, frame level
// @output:
// ================================================================================
void Vis::add_tree_node(LitVec cube, size_t level) {
  // new node
  uc_tree_node *temp_node = new uc_tree_node();
  temp_node->cube = cube;
  if (level == 1) {
    bool has_same = false;
    for (uc_tree_node *bro : uc_tree_root->children) {
      if (_cube_same(bro->cube, temp_node->cube)) {
        has_same = true;
        break;
      }
    }
    if (!has_same) uc_tree_root->children.emplace_back(temp_node);
  } else {
    int depth = 1;
    uc_tree_node *temp_anc = uc_tree_root;
    while (depth < level) {
      bool has_anc = false;
      for (uc_tree_node *anc : temp_anc->children) {
        if (_cube_imply(anc->cube, temp_node->cube)) {
          temp_anc = anc;
          has_anc = true;
          break;
        }
      }
      if (!has_anc) break;
      depth++;
    }
    if (depth == level) {
      bool has_same = false;
      for (uc_tree_node *bro : temp_anc->children) {
        if (_cube_same(bro->cube, temp_node->cube)) {
          has_same = true;
          break;
        }
      }
      if (!has_same) temp_anc->children.emplace_back(temp_node);
      if (_cube_same(temp_node->cube, temp_anc->cube)) {
        temp_node->is_same = true;
        temp_anc->is_same = true;
      }
    } else {
      while (depth <= level) {
        temp_anc->children.emplace_back(temp_node);
        temp_anc = temp_node;
        temp_node->is_same = true;
        temp_node = new uc_tree_node();
        temp_node->cube = cube;
        depth++;
      }
    }
  }
  return;
}

std::string _cube_to_string(LitVec cube) {
  std::string res = "";
  for (auto l : cube) {
    res += std::to_string(l.x) + "\n";
  }
  return res;
}

void Vis::print_tree(size_t k) {
  // io
  std::ofstream visFile;
  visFile.open("frame_vis/k" + to_string(k) + ".gml");
  // header
  visFile << "Creator\t"
          << "\"car visualization\"" << std::endl;
  visFile << "Version\t" << 0.1 << std::endl;
  visFile << "graph" << std::endl;
  visFile << "[" << std::endl;
  // graph info
  visFile << "directed\t" << 1 << std::endl;
  // traverse tree
  uint tree_id = 0;
  std::queue<uint> id_queue;
  std::queue<uc_tree_node *> node_queue;
  id_queue.push(tree_id);
  node_queue.push(uc_tree_root);
  write_node(visFile, tree_id, "root");
  tree_id++;

  while (!node_queue.empty()) {
    uc_tree_node *temp_node = node_queue.front();
    node_queue.pop();
    uint temp_id = id_queue.front();
    id_queue.pop();
    for (auto node : temp_node->children) {
      id_queue.push(tree_id);
      node_queue.push(node);
      if (node->is_same) {
        write_node(visFile, tree_id, _cube_to_string(node->cube));
      } else {
        write_node(visFile, tree_id, _cube_to_string(node->cube), "\"#ffffff\"");
      }
      write_edge(visFile, temp_id, tree_id);
      tree_id++;
    }
  }
  visFile << "]" << std::endl;
  visFile.close();
}
