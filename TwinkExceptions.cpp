#include "TwinkExceptions.h"

AlreadyRunningException::AlreadyRunningException() : MyException{ "Already running" } {}; // Function body left blank intentionally
CreateMutexException::CreateMutexException() : MyException{ "Create Mutex" } {}; // Function body left blank intentionally
CloseMutexException::CloseMutexException() : MyException{ "Close Mutex" } {}; // Function body left blank intentionally
PopupException::PopupException() : MyException{ "Popup" } {}; // Function body left blank intentionally
OpenRegistryException::OpenRegistryException() : MyException{ "Open Registry" } {}; // Function body left blank intentionally
SetRegistryException::SetRegistryException() : MyException{ "Set Registry" } {}; // Function body left blank intentionally
GetModuleException::GetModuleException() : MyException{ "Get Module" } {}; // Function body left blank intentionally