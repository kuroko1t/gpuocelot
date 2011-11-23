/*! \file PTXInstrumentor.h
	\date Saturday November 13, 2010
	\author Naila Farooqui <naila@cc.gatech.edu>
	\brief The header file for the Ocelot PTX Instrumentor
*/

#ifndef PTX_INSTRUMENTOR_H_INCLUDED
#define PTX_INSTRUMENTOR_H_INCLUDED

#include <string>
#include <vector>
#include <map>

#include <ocelot/analysis/interface/kernel_profile.h>
#include <ocelot/ir/interface/Module.h>
#include <ocelot/transforms/interface/Pass.h>

#include <hydrazine/implementation/json.h>

#include <ostream>

#include <mqueue.h>

namespace analysis
{
    /*! \brief Captures profile data about an executed kernel */
	class KernelProfile
	{
		public:

            /*! \brief Maps CTA->(clock cycles, SM) */
            typedef std::map<size_t, std::vector<size_t>> ThreadBlockToProcessorMap;

            /*! \brief Maps number of CTAs executed on each SM */
            typedef std::map<size_t, size_t> ProcessorToThreadBlockCountMap;
        
            /*! \brief Maps SM ID to total number of clock cycles */
            typedef std::map<size_t, size_t> ProcessorToClockCyclesMap;

            /*! \brief Maps Basic Block ID to total number of basic block execution */
            typedef std::map<size_t, size_t> BasicBlockToExecCountMap;
       

            /*! \brief The name of the kernel */
            std::string name;

            /*! \brief max kernel execution time for a processor (SM) */
            double maxSMRuntime;
            
            /*! \brief Dynamic instruction count */
            double instructionCount;

            /*! \brief Maps CTA->(clock cycles, SM) */
            ThreadBlockToProcessorMap threadBlockToProcessorMap;

            ProcessorToClockCyclesMap processorToClockCyclesMap;

            /*! \brief Maps number of CTAs executed on each SM */
            ProcessorToThreadBlockCountMap processorToThreadBlockCountMap;

            BasicBlockToExecCountMap basicBlockExecutionCountMap;
            BasicBlockToExecCountMap memoryOperationsMap;
	};

	/*! \brief Able to run various instrumentation passes over PTX modules */
	class PTXInstrumentor
	{
		
        public:

            enum FormatType {
                json,
                text
            };
        
            typedef std::map<int, transforms::Pass *> PassMap;
            typedef std::map<std::string, size_t> KernelDataMap;
            typedef std::map<std::string, std::vector<std::string>> KernelLabelsMap;

			/*! \brief device name for JSON output */
			std::string deviceName;
			
			/*! \brief The input file being instrumented */
			std::string input;
			
			/*! \brief The output file being generated */
			std::string output;

            /*! \brief The output stream for writing instrumentation results */
            std::ostream *out;
            
            /*! \brief The PTX module being instrumented */
            ir::Module *module;
            
            /*! \brief The name of kernel being instrumented */
			std::string kernelName;        

            /*! \brief The instrumentation passes */
            PassMap passes;
            
            /*! \brief The symbol name for data structure storing instrumentation results */
            std::string symbol;
            
            /*! \brief The number of thread blocks or CTAs specified for this execution */
            unsigned int threadBlocks;

            /*! \brief The number of threads specified for this execution */
            unsigned int threads;

            /*! \brief The strings representing the basic block labels */
            KernelLabelsMap kernelLabelsMap;
            
            /*! \brief make sure all conditions have been met to perform this instrumentation */
            bool conditionsMet;
    
             /*! \brief format for display */
            FormatType fmt;

            /*! \brief check if device info has already been written */
            bool deviceInfoWritten;
            
            unsigned int sharedMemSize;
            
            KernelDataMap kernelDataMap;

        protected:
            
            KernelProfile _kernelProfile;
            
            kernel_profile _profile;
			
		public:
		
		    PTXInstrumentor();
            ~PTXInstrumentor();
		    
            /*! \brief The checkConditions method verifies that the defined conditions are met for this instrumentation */
            virtual void checkConditions() = 0;

            /*! \brief The analyze method performs any necessary static analysis */
            virtual void analyze(ir::Module &module) = 0;

            /*! \brief Performs the instrumentation */
			void instrument();		

            /*! \brief Performs the instrumentation */
            void instrument(ir::Module& module);	

            /*! \brief The initialize method performs any necessary CUDA runtime initialization prior to instrumentation */
            virtual void initialize() = 0;

            /*! \brief The finalize method performs any necessary CUDA runtime actions after instrumentation */
            void finalize();

            /*! \brief The createPasses method instantiates the instrumentation passes */
            virtual void createPasses() = 0;

            /*! \brief Output device info */
            void deviceInfo(std::ostream *out);

            /*! \brief The jsonEmitter method creates a JSON emitter to display JSON */
            void jsonEmitter(std::string metric, hydrazine::json::Object *stats);
			
            /*! \brief Extracts instrumentation-specific data */
            virtual void extractResults(std::ostream *out) = 0;

            /*! \brief obtain kernel profile */
            KernelProfile kernelProfile();
            
            /*! \brief send kernel profile data */
            int sendKernelProfile(mqd_t messageQueue);
	};

    typedef std::vector< PTXInstrumentor *> PTXInstrumentorVector;
}

#endif
