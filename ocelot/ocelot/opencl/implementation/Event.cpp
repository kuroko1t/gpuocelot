#include <ocelot/opencl/interface/OpenCLRuntime.h>
#include <ocelot/opencl/interface/Event.h>

#undef REPORT_BASE
#define REPORT_BASE 0

opencl::Event::Event(cl_command_type type, 
	CommandQueue * commandQueue, 
	Context * context,
    cl_uint num_events_in_wait_list,
    const cl_event * event_wait_list,
    cl_event * event): Object(OBJTYPE_EVENT),
	_type(type), _commandQueue(commandQueue), _context(context) {

	report("Creating Event " << this);
	if(event_wait_list == NULL && num_events_in_wait_list > 0)
		throw CL_INVALID_EVENT_WAIT_LIST;

	if(event_wait_list && num_events_in_wait_list == 0)
		throw CL_INVALID_EVENT_WAIT_LIST;

	for(cl_uint i = 0; i < num_events_in_wait_list; i++) {
		if(!event_wait_list[i]->isValidObject(Object::OBJTYPE_EVENT)) {
			_waitList.clear();
			throw CL_INVALID_EVENT_WAIT_LIST;
		}
		event_wait_list[i]->retain();
		_waitList.push_back(event_wait_list[i]);
	}

	if(_commandQueue) //user event does not have commandQueue
		_commandQueue->retain();

	_context->retain();

	if(event) {
		this->retain();
		*event = (cl_event)this;
	}
}

opencl::Event::~Event() {

	report("Delete event " << this);

	if(_commandQueue && _commandQueue->release())
		delete _commandQueue;

	if(_context->release())
		delete _context;

	for(EventList::iterator event = _waitList.begin(); 
		event != _waitList.end(); event++) {
		if((*event)->release())
			delete *event;
	}
}

cl_command_type opencl::Event::type() {
	return _type;
}

bool opencl::Event::isCompleted() {
	return ((_status == CL_COMPLETE) ||
		(_status < 0));
}

bool opencl::Event::hasStatus(cl_int status) {
	return (_status == status);
}

void opencl::Event::setStatus(cl_int status) {
	_status = status;
}


opencl::ReadBufferEvent::ReadBufferEvent(CommandQueue * commandQueue,
	BufferObject * buffer,
	cl_bool blockingRead,
	size_t offset, size_t cb, void * ptr,
	cl_uint num_events_in_wait_list,
	const cl_event * event_wait_list,
	cl_event * event) :
	Event(CL_COMMAND_READ_BUFFER, commandQueue, 
	commandQueue->context(), num_events_in_wait_list,
	event_wait_list, event),
	_buffer(buffer), _offset(offset),
	_cb(cb), _ptr(ptr) {

	if(offset >= buffer->size() || cb + offset > buffer->size())
		throw CL_INVALID_VALUE;
	
	if(ptr == NULL)
		throw CL_INVALID_VALUE;

	if(!buffer->isAllocatedOnDevice(commandQueue->device()))
		throw CL_MEM_OBJECT_ALLOCATION_FAILURE;

	commandQueue->queueEvent(this, blockingRead);
}

opencl::ReadBufferEvent::~ReadBufferEvent() {
}

void opencl::ReadBufferEvent::execute(Device * device) {
	assertM(device == _commandQueue->device(), "invalid device");
	_buffer->readOnDevice(device, _offset, _cb, _ptr);
}
