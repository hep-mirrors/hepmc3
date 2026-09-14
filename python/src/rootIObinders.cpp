#include "rootIObinders.h"
#include "Rtypes.h"

namespace binder {


	void	WriterRootTree_binder(pybind11::module &M)
{
	pybind11::class_<HepMC3::WriterRootTree, std::shared_ptr<HepMC3::WriterRootTree>, HepMC3::Writer> cl(M, "WriterRootTree", "");
	pybind11::handle cl_type = cl;

	cl.def( pybind11::init( [](const std::string & filename)
	{
		return new HepMC3::WriterRootTree(filename);
	}),
	pybind11::arg("filename"), "doc");

	cl.def( pybind11::init<	const std::string &, std::shared_ptr<HepMC3::GenRunInfo>>(),
			pybind11::arg("filename"), pybind11::arg("run") );

	cl.def( pybind11::init<	const std::string &, std::shared_ptr<HepMC3::GenRunInfo>, bool>(),
			pybind11::arg("filename"), pybind11::arg("run"), pybind11::arg("append") );

	cl.def( pybind11::init( [](const std::string & filename, bool append)
	{
		return new HepMC3::WriterRootTree(filename, std::shared_ptr<HepMC3::GenRunInfo>(), append);
	}),
	pybind11::arg("filename"), pybind11::arg("append"), "doc");

	cl.def( pybind11::init( [](const std::string & filename,
							   const std::string & treename,
							   const std::string & branchname){
		return new HepMC3::WriterRootTree(filename, treename, branchname);
	}),
	pybind11::arg("filename"), pybind11::arg("treename"),
	pybind11::arg("branchname"), "doc");

	cl.def( pybind11::init<const std::string &, const std::string &, const std::string &,
						   std::shared_ptr<HepMC3::GenRunInfo>>(),
			pybind11::arg("filename"), pybind11::arg("treename"),
			pybind11::arg("branchname"), pybind11::arg("run") );

	cl.def( pybind11::init<	const std::string &, const std::string &,
							const std::string &, std::shared_ptr<HepMC3::GenRunInfo>,
							bool>(),
			pybind11::arg("filename"), pybind11::arg("treename"),
			pybind11::arg("branchname"), pybind11::arg("run"),
			pybind11::arg("append") );

	cl.def( pybind11::init( [](	const std::string & filename,
								const std::string & treename,
								const std::string & branchname,
								bool append)
	{
		return new HepMC3::WriterRootTree(filename, treename, branchname, std::shared_ptr<HepMC3::GenRunInfo>(), append);
	}),
	pybind11::arg("filename"), pybind11::arg("treename"), pybind11::arg("branchname"), pybind11::arg("append"), "doc");

	cl.def( pybind11::init<const std::string &, const std::string &, const std::string &,
						   std::shared_ptr<HepMC3::GenRunInfo>, bool>(),
			pybind11::arg("filename"), pybind11::arg("treename"),
			pybind11::arg("branchname"), pybind11::arg("run"), pybind11::arg("append") );

		cl.def("write_event", (void (HepMC3::WriterRootTree::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterRootTree::write_event, "Write event to file\n\n  \n Event to be serialized\n\nC++: HepMC3::WriterRootTree::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("write_run_info", (void (HepMC3::WriterRootTree::*)()) &HepMC3::WriterRootTree::write_run_info, "Write the GenRunInfo object to file. \n\nC++: HepMC3::WriterRootTree::write_run_info() --> void");
		cl.def("close", (void (HepMC3::WriterRootTree::*)()) &HepMC3::WriterRootTree::close, "Close file stream \n\nC++: HepMC3::WriterRootTree::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterRootTree::*)()) &HepMC3::WriterRootTree::failed, "Get stream error state flag \n\nC++: HepMC3::WriterRootTree::failed() --> bool");
	}

	void	WriterRoot_binder(pybind11::module &M)
	{
			pybind11::class_<HepMC3::WriterRoot, std::shared_ptr<HepMC3::WriterRoot>,  HepMC3::Writer> cl(M, "WriterRoot", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init( [](const class std::basic_string<char> & a0){ return new HepMC3::WriterRoot(a0); }/*, [](const class std::basic_string<char> & a0){ return new PyCallBack_HepMC3_WriterRoot(a0); } */), "doc");
		cl.def( pybind11::init<const std::string &, class std::shared_ptr<class HepMC3::GenRunInfo>>(), pybind11::arg("filename"), pybind11::arg("run") );

		cl.def("write_event", (void (HepMC3::WriterRoot::*)(const class HepMC3::GenEvent &)) &HepMC3::WriterRoot::write_event, "Write event to file\n\n  \n Event to be serialized\n\nC++: HepMC3::WriterRoot::write_event(const class HepMC3::GenEvent &) --> void", pybind11::arg("evt"));
		cl.def("write_run_info", (void (HepMC3::WriterRoot::*)()) &HepMC3::WriterRoot::write_run_info, "Write the GenRunInfo object to file. \n\nC++: HepMC3::WriterRoot::write_run_info() --> void");
		cl.def("close", (void (HepMC3::WriterRoot::*)()) &HepMC3::WriterRoot::close, "Close file stream \n\nC++: HepMC3::WriterRoot::close() --> void");
		cl.def("failed", (bool (HepMC3::WriterRoot::*)()) &HepMC3::WriterRoot::failed, "Get stream error state flag \n\nC++: HepMC3::WriterRoot::failed() --> bool");
	}
	void	ReaderRootTree_binder(pybind11::module &M)
	{
			pybind11::class_<HepMC3::ReaderRootTree, std::shared_ptr<HepMC3::ReaderRootTree>,  HepMC3::Reader> cl(M, "ReaderRootTree", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def( pybind11::init<const std::string &, const std::string &, const std::string &>(), pybind11::arg("filename"), pybind11::arg("treename"), pybind11::arg("branchname") );

		cl.def("skip", (bool (HepMC3::ReaderRootTree::*)(const int)) &HepMC3::ReaderRootTree::skip, "skip events\n\nC++: HepMC3::ReaderRootTree::skip(const int) --> bool", pybind11::arg(""));

		cl.def("read_event", (bool (HepMC3::ReaderRootTree::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderRootTree::read_event, "Read event from file\n\n  \n Contains parsed event\n\nC++: HepMC3::ReaderRootTree::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));

		// read_event method currently unique to ReaderRootTree, with an index parameter
		cl.def("read_event_at_index",
			static_cast<bool (HepMC3::ReaderRootTree::*)(HepMC3::GenEvent &, const Long64_t)>(&HepMC3::ReaderRootTree::read_event_at_index),
			"Read event from file at specific index\n\n  \n Contains parsed event\n  \n Event index\n\nC++: HepMC3::ReaderRootTree::read_event_at_index(class HepMC3::GenEvent &, const int) --> bool",
			pybind11::arg("evt"), pybind11::arg("index"));

		cl.def("close", (void (HepMC3::ReaderRootTree::*)()) &HepMC3::ReaderRootTree::close, "Close file \n\nC++: HepMC3::ReaderRootTree::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderRootTree::*)()) &HepMC3::ReaderRootTree::failed, "Get file  error state \n\nC++: HepMC3::ReaderRootTree::failed() --> bool");

}

	void	ReaderRoot_binder(pybind11::module &M)
	{
			pybind11::class_<HepMC3::ReaderRoot, std::shared_ptr<HepMC3::ReaderRoot>,  HepMC3::Reader> cl(M, "ReaderRoot", "");
		pybind11::handle cl_type = cl;

		cl.def( pybind11::init<const std::string &>(), pybind11::arg("filename") );

		cl.def("skip", (bool (HepMC3::ReaderRoot::*)(const int)) &HepMC3::ReaderRoot::skip, "skip events\n\nC++: HepMC3::ReaderRoot::skip(const int) --> bool", pybind11::arg(""));

		cl.def("read_event", (bool (HepMC3::ReaderRoot::*)(class HepMC3::GenEvent &)) &HepMC3::ReaderRoot::read_event, "Read event from file\n\n  \n Contains parsed event\n\nC++: HepMC3::ReaderRoot::read_event(class HepMC3::GenEvent &) --> bool", pybind11::arg("evt"));
		cl.def("close", (void (HepMC3::ReaderRoot::*)()) &HepMC3::ReaderRoot::close, "Close file stream \n\nC++: HepMC3::ReaderRoot::close() --> void");
		cl.def("failed", (bool (HepMC3::ReaderRoot::*)()) &HepMC3::ReaderRoot::failed, "Get stream error state \n\nC++: HepMC3::ReaderRoot::failed() --> bool");

}

} // namespace binder
