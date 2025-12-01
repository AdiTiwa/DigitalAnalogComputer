#ifndef XYCE_WRAPPER_H
#define XYCE_WRAPPER_H

#include <string>

class XyceWrapper {
public:
    XyceWrapper(const std::string& template_path, const std::string& work_dir);
    ~XyceWrapper();

    // Run one time step
    // input_voltage: The voltage value from the digital DAC
    // current_cap_voltage: The current state of the capacitor (to be set as IC)
    // Returns: The new voltage across the capacitor
    double run_step(double input_voltage, double current_cap_voltage);

private:
    std::string template_path_;
    std::string work_dir_;
    std::string netlist_path_;
    std::string output_path_;
    
    // Helper to read file content
    std::string read_file(const std::string& path);
    // Helper to write file content
    void write_file(const std::string& path, const std::string& content);
    // Helper to parse Xyce output
    double parse_output(const std::string& path);
};

#endif // XYCE_WRAPPER_H
