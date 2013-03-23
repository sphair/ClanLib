
#include "precomp.h"
#include "glsl_program.h"

using namespace clan;

GlslProgram::GlslProgram()
: _context(0), _module(0), _engine(0), pass_manager(0)
{
	llvm::llvm_start_multithreaded();
	llvm::InitializeNativeTarget();
	_context = new llvm::LLVMContext();
	_module = new llvm::Module("glsl", context());
	std::string error_string;
	llvm::EngineBuilder engine_builder(_module); // transfers module ownership to engine builder
	engine_builder.setErrorStr(&error_string);
	engine_builder.setOptLevel(llvm::CodeGenOpt::Aggressive);
	engine_builder.setRelocationModel(llvm::Reloc::Static);
	_engine = engine_builder.create();
	if (_engine == 0)
		throw Exception(string_format("Failed to create LLVM execution engine: %1", error_string));

	pass_manager = new llvm::PassManager();
	pass_manager->add(new llvm::DataLayout(*_engine->getDataLayout()));

	llvm::PassManagerBuilder pass_manager_builder;
	pass_manager_builder.OptLevel = 3;
	pass_manager_builder.SizeLevel = 0;
	pass_manager_builder.Inliner = llvm::createAlwaysInlinerPass();
	//pass_manager_builder.Inliner = llvm::createFunctionInliningPass();
	pass_manager_builder.populateModulePassManager(*pass_manager);
}

GlslProgram::~GlslProgram()
{
	delete pass_manager;
	delete _engine;
	delete _context;
	llvm::llvm_stop_multithreaded();
}

void *GlslProgram::compile(std::string name)
{
	//_module->dump();
	pass_manager->run(*_module);

	llvm::Function *function = _module->getFunction(name.c_str());
	if (function == 0)
		throw Exception("Function not found");
	return _engine->getPointerToFunction(function);
}
