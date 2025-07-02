#pragma once

#include "MyException.h"


class AlreadyRunningException : public MyException {
public:
	AlreadyRunningException();
};

class CreateMutexException : public MyException {
public:
	CreateMutexException();
};

class CloseMutexException : public MyException {
public:
	CloseMutexException();
};

class PopupException : public MyException {
public:
	PopupException();
};

class OpenRegistryException : public MyException {
public:
	OpenRegistryException();
};

class SetRegistryException : public MyException {
public:
	SetRegistryException();
};

class GetModuleException : public MyException {
public:
	GetModuleException();
};