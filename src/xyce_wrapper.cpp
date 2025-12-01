#include "xyce_wrapper.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

XyceWrapper::XyceWrapper(const std::string &template_path,
                         const std::string &work_dir)
    : template_path_(template_path), work_dir_(work_dir) {
  netlist_path_ = work_dir_ + "/run.cir";
  output_path_ = work_dir_ + "/run.cir.prn";
}

XyceWrapper::~XyceWrapper() {
  // Cleanup if needed
}

std::string XyceWrapper::read_file(const std::string &path) {
  std::ifstream t(path);
  if (!t.is_open()) {
    std::cerr << "Error: Could not open " << path << std::endl;
    return "";
  }
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

void XyceWrapper::write_file(const std::string &path,
                             const std::string &content) {
  std::ofstream t(path);
  t << content;
  t.close();
}

double XyceWrapper::run_step(double input_voltage, double current_cap_voltage) {
  // 1. Read template
  std::string content = read_file(template_path_);

  // 2. Replace placeholders
  std::string ic_str = ".IC V(2)=" + std::to_string(current_cap_voltage);
  std::string v_str = std::to_string(input_voltage);

  size_t ic_pos = content.find("%IC_PLACEHOLDER%");
  if (ic_pos != std::string::npos) {
    content.replace(ic_pos, 16, ic_str);
  }

  size_t v_pos = content.find("%VOLTAGE_PLACEHOLDER%");
  if (v_pos != std::string::npos) {
    content.replace(v_pos, 21, v_str);
  }

  // 3. Write netlist
  write_file(netlist_path_, content);

  // 4. Run Xyce
  // Redirect output to /dev/null to keep console clean, or capture it if
  // debugging
  std::string cmd = "Xyce " + netlist_path_ + " > /dev/null 2>&1";
  int ret = system(cmd.c_str());
  if (ret != 0) {
    std::cerr << "Error: Xyce failed to run." << std::endl;
    return current_cap_voltage; // Fallback
  }

  // 5. Parse output
  return parse_output(output_path_);
}

double XyceWrapper::parse_output(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open output file " << path << std::endl;
    return 0.0;
  }

  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  // The output format of Xyce .PRINT TRAN is typically:
  // Index TIME V(2)
  // 0 0.000000e+00 1.000000e+00
  // 1 1.000000e-06 1.000000e+00
  // We want the last line's voltage (last column)

  // Search backwards for a valid data line
  // Skip empty lines and "End of Xyce" messages
  std::string last_line;
  bool found = false;
  for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
    if (it->empty())
      continue;
    if (it->find("End of Xyce") != std::string::npos)
      continue;
    // Check if it starts with a digit (index)
    if (isdigit((*it)[0])) {
      last_line = *it;
      found = true;
      break;
    }
  }

  if (!found)
    return 0.0;

  std::stringstream ss(last_line);
  std::string segment;
  std::vector<std::string> seglist;

  while (std::getline(ss, segment, ' ')) {
    if (!segment.empty()) {
      seglist.push_back(segment);
    }
  }

  // Assuming the last column is the voltage
  // Sometimes Xyce output might be tab separated or have multiple spaces
  // The above loop handles multiple spaces if they are just delimiters
  // But we need to be careful about the header line if the simulation
  // failed/empty

  if (seglist.size() < 2)
    return 0.0;

  try {
    return std::stod(seglist.back());
  } catch (...) {
    return 0.0;
  }
}
