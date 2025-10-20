#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkXMLPolyDataWriter.h>

#include <filesystem>

#include "SPICE.hpp"
#include "fileTool.hpp"

//SPICE CF; 

bool try_to_readConf(int num, SPICE &CF, int &OKNum) {
  char file_name[256];
  snprintf(file_name, 256, "conf%d", num);
  if (fileTool::fileExists(file_name)) {
    OKNum = num;
    CF.loadConf(file_name);
    CF.accelerations();
    return true;
  } else {
    std::cout << file_name << " does not exist" << std::endl;
  }
  return false;
}

void writeParticlesVTP(const std::vector<Particle>& particles, float t, const std::string& file_name) {
  
    vtkNew<vtkPoints> points;
    vtkNew<vtkFloatArray> velArr; velArr->SetName("velocity"); velArr->SetNumberOfComponents(3);
    vtkNew<vtkFloatArray> accArr; accArr->SetName("acceleration"); accArr->SetNumberOfComponents(3);
    vtkNew<vtkFloatArray> forceArr; forceArr->SetName("force"); forceArr->SetNumberOfComponents(3);
    vtkNew<vtkFloatArray> radiusArr; radiusArr->SetName("radius"); radiusArr->SetNumberOfComponents(1);
    vtkNew<vtkFloatArray> massArr; massArr->SetName("mass"); massArr->SetNumberOfComponents(1);
    
    for (const auto& p : particles) {
        points->InsertNextPoint(p.pos.x, p.pos.y, 0.0);
        velArr->InsertNextTuple3(p.vel.x, p.vel.y, 0.0);
        accArr->InsertNextTuple3(p.acc.x, p.acc.y, 0.0);
        forceArr->InsertNextTuple3(p.force.x, p.force.y, 0.0);
        radiusArr->InsertNextValue(p.radius);
        massArr->InsertNextValue(p.mass);
    }

    vtkNew<vtkPolyData> poly;
    poly->SetPoints(points);
    poly->GetPointData()->AddArray(velArr);
    poly->GetPointData()->AddArray(accArr);
    poly->GetPointData()->AddArray(forceArr);
    poly->GetPointData()->AddArray(radiusArr);
    poly->GetPointData()->AddArray(massArr);

    vtkNew<vtkXMLPolyDataWriter> writer;
    writer->SetFileName(file_name.c_str());
    writer->SetInputData(poly);
    writer->SetDataModeToBinary();
    writer->Write();
}

void writePVD(const std::vector<std::string>& files,
              const std::vector<float>& times,
              const std::string& pvdName) {
    std::ofstream ofs(pvdName);
    ofs << "<?xml version=\"1.0\"?>\n";
    ofs << "<VTKFile type=\"Collection\" version=\"1.0\" byte_order=\"LittleEndian\">\n";
    ofs << "  <Collection>\n";

    for (size_t i = 0; i < files.size(); ++i) {
        // Use high precision for time
        ofs << "    <DataSet timestep=\"" 
            << std::fixed << std::setprecision(6) << times[i] 
            << "\" file=\"" << files[i] << "\"/>\n";
    }
    ofs << "  </Collection>\n";
    ofs << "</VTKFile>\n";
    std::cout << "Wrote PVD file: " << pvdName << " with " << files.size() << " timesteps.\n";
}


int main(int argc, char *argv[]) {

    SPICE Conf;
    int confNum = 0;
    std::vector<std::string> vtpFiles;
    std::vector<float> times;
    std::string arg1 = argv[1];
    std::string outputDirvtp = "./vtp";  // Default directory

    if (argc != 2) {
        std::cout << "  Missing argument 1 : 'all' or file-name\n"
                  << "  EXIT\n";
        return 0;
    }
    else if (argc == 2) {

        if (!std::filesystem::exists(outputDirvtp)) {
            std::filesystem::create_directories(outputDirvtp);
            std::cout << "Created output directory: " << outputDirvtp << std::endl;
        }
        if (arg1 == "all") {
            confNum = 0;
            while (try_to_readConf(confNum, Conf, confNum)) {
                char file_name_vtp[512];
                snprintf(file_name_vtp, sizeof(file_name_vtp), "%s/conf%d.vtp", outputDirvtp.c_str(), confNum);
                writeParticlesVTP(Conf.Particles, Conf.t, file_name_vtp);
                std::cout << " -> Wrote " << file_name_vtp << std::endl;
                vtpFiles.push_back(file_name_vtp);
                times.push_back(Conf.t);
                confNum++;
            }
            writePVD(vtpFiles, times, "paraview.pvd");
        }
        else {
            Conf.loadConf(argv[1]);
            std::cout << "Loaded configuration file: " << argv[1] << std::endl;
            char file_name_vtp[512];
            snprintf(file_name_vtp, sizeof(file_name_vtp), "%s/%s.vtp", outputDirvtp.c_str(), argv[1]);
            writeParticlesVTP(Conf.Particles, Conf.t, file_name_vtp);
            std::cout << " -> Wrote " << file_name_vtp << std::endl;
        }
    }

    return 0;
}
