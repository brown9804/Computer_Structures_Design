
#include "pBimodal.h"

bimodal_F::bimodal_F(std::string filepath,int s, int gh, int ph){
    this->filepath=filepath; //Compress file entry
    this->s=s; // Branch History Table (BHT) size
    this->gh=gh; //GShare size (given value)
    this->ph=ph; //PShare size (given value)
    };

void bimodal_F::begin_prediction_B(){
  int Table_Nentry = pow(2, s); // 2^s
  int counter_miss_T = 0; // Incorrect Taken branches
  int counter_miss_N = 0; // Incorrect Not Taken branches
  int counter_T = 0; // Right Taken branches
  int counter_N = 0; // Right Not Taken branches
  char *Table = (char *)calloc(Table_Nentry, sizeof(char));
  long mask = Table_Nentry - 1; // mask size for redirecting network
  std::string op_mode = "Bimodal";
  int num_branch = 0; // number of branches (jumps)
  /////////
  time_t current_time;
  time_t final_time;

  time(&current_time); // capture time
  std::ofstream results_file ("results/result_sum_bimodal.txt", std::ofstream::out);       // result file write
  if(!results_file) // if no file
    {
         std::cout<<"ERROR, there is no file created ............. ERROR ..... ERROR";
    }
  /////// File open
  for (std::string line; std::getline(std::cin, line);) {
    long pc_dd = 0;
    char result = ' ';
    sscanf(line.c_str(), "%ld %c", &pc_dd, &result);

    // std::string pc_dd_10 = line.substr(0,10);  // Decimal direction // position // how many
    // char result = line.substr(11,1).c_str()[0];    // Result? -> T or N
    // long pc_dd = (int)std::strtol(pc_dd_10.c_str(), nullptr, 10); // 10 is the base which parse the string

    if ((result != 'N') && (result != 'T')) {
      std::cout << "Parser error at line " << num_branch << std::endl;
    }

    long pc_mask_access = pc_dd & mask;

    //std::cout << "Address " << pc_dd << " mapped to " << pc_mask_access << " with mask " << mask << std::endl;

    if (Table[pc_mask_access] <= 0) { 
      if (result == 'N') {
        counter_N++;
      } else {
        counter_miss_N++;
      }
    } else {
      if (result == 'T') {
        counter_T++;
      } else {
        counter_miss_T++;
      }
    }

    // Adjust counters based on result
    if (result == 'N') {
      // Decrement if not 0
      if (Table[pc_mask_access] > 0) {
        Table[pc_mask_access]--;
      }
    } else {
      // Increment if 3
      if (Table[pc_mask_access] < 3) {
        Table[pc_mask_access]++;
      }
    }

    num_branch++; // count all lines
  }

  time(&final_time); // capture time
  int ex_time = difftime(final_time,current_time);
  float percentage = (float)(counter_T + counter_N)*(float)(100/((float)num_branch)); // for all lines

  /////////////////////Results////////////////////////////////////////////////////
  results_file << "------------------------------------------------------------------------ \n";
  results_file << "Prediction parameters: \n ";
  results_file << "------------------------------------------------------------------------ \n";
  results_file << "Branch prediction type: " << op_mode << std::endl;
  results_file << "BHT size (entries): " << s << std::endl;
  results_file << "Global history register size: " << gh << std::endl;
  results_file << "Private history register size: " << ph << std::endl;
  results_file << "------------------------------------------------------------------------ \n";
  results_file << "Simulation results: " << std::endl;
  results_file << "------------------------------------------------------------------------\n";
  results_file << "Number of branch: " << num_branch << std::endl;
  results_file << "Number of correct prediction of taken branches: " << counter_T << std::endl;
  results_file << "Number of incorrect prediction of taken branches: " << counter_miss_T << std::endl;
  results_file << "Correct prediction of not taken branches: " << counter_N << std::endl;
  results_file << "Incorrect prediction of not taken branches: " << counter_miss_N << std::endl;
  results_file << "Percentage of correct predictions: " << percentage << "%" <<std::endl;
  results_file << "Time running: " << ex_time << std::endl;

  /////////////////////Results////////////////////////////////////////////////////
  std::cout << "------------------------------------------------------------------------ \n";
  std::cout << "Prediction parameters: \n ";
  std::cout << "------------------------------------------------------------------------ \n";
  std::cout << "Branch prediction type: " << op_mode << std::endl;
  std::cout << "BHT size (entries): " << s << std::endl;
  std::cout << "Global history register size: " << gh << std::endl;
  std::cout << "Private history register size: " << ph << std::endl;
  std::cout << "------------------------------------------------------------------------ \n";
  std::cout << "Simulation results: " << std::endl;
  std::cout << "------------------------------------------------------------------------\n";
  std::cout << "Number of branch: " << num_branch << std::endl;
  std::cout << "Number of correct prediction of taken branches: " << counter_T << std::endl;
  std::cout << "Number of incorrect prediction of taken branches: " << counter_miss_T << std::endl;
  std::cout << "Correct prediction of not taken branches: " << counter_N << std::endl;
  std::cout << "Incorrect prediction of not taken branches: " << counter_miss_N << std::endl;
  std::cout << "Percentage of correct predictions: " << percentage << "%" <<std::endl;
  std::cout << "Time running: " << ex_time << " seconds" << std::endl;
  //
  // std::cout << "\n\n          *****           \n\n" << std::endl;
  // std::cout << "The number of branches analyzed is " << stop_analize << " so consider that in this results " << std::endl;
  // std::cout << "If you want more lines review the code and remove: stop_analize condition " << std::endl;
  //
  free(Table);
} // end all
