#pragma once
#include <crtdbg.h>

//*****************************************************************************
//*                                                                           *
//*                               Definitions                                 *
//*                                                                           *
//*  Definition for Global use of Application                                 *
//*                                                                           *
//*****************************************************************************
#define	INLINE												inline

#ifndef PURE
#define	PURE												= 0
#endif

#ifndef EMPTY
#define	EMPTY												{}
#endif

#ifndef EMPTY_TRUE
#define	EMPTY_TRUE											{	return TRUE;}
#endif

#ifndef EMPTY_FALSE
#define	EMPTY_FALSE											{	return FALSE;}
#endif

#ifndef EMPTY_ZERO
#define	EMPTY_ZERO											{	return 0;}
#endif


#ifndef SAFE_FREE
#define	SAFE_FREE(p)										if(p){ free(p);p=NULL;}
#endif

#ifndef SAFE_DELETE
#define	SAFE_DELETE(p)										if(p){ delete p; p=NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)								{ if(p) { delete[] (p);   (p) = NULL; } }
#endif

#ifndef SAFE_RELEASE
#define	SAFE_RELEASE(data)									if(data){ (data)->Release();(data)=NULL;}
#endif

#ifndef SAFE_CLOSEHANDLE
#define	SAFE_CLOSEHANDLE(data)								if(data!=INVALID_HANDLE_VALUE){ CloseHandle(data);data=INVALID_HANDLE_VALUE;}
#endif


#ifndef ASSERT
#define ASSERT(f)										_ASSERT(f)
#endif

#ifndef TRACE
//#define	TRACE											CGNetTrace
#define	TRACE											__noop
#endif


// ----------------------------------------------------------------------------
// Trace & Assert
// ----------------------------------------------------------------------------
// 1) �⺻ Trace
#define	CGTRACE											TRACE


// 2) Error�� ����� ���� Trace
#define	_USE_TRACE_ERROR
#ifdef _USE_TRACE_ERROR
#define	CGTRACE_ERROR								TRACE
#else
#define	CGTRACE_ERROR								__noop
#endif

// 3) Error�� ����� ���� Trace
#define	_USE_TRACE_IMPORTANT
#ifdef _USE_TRACE_IMPORTANT
#define	CGTRACE_ERROR_IMPORTANT						TRACE
#else
#define	CGTRACE_ERROR_IMPORTANT						__noop
#endif

// 4) Info�� ����� ���� Trace
//#define	_USE_TRACE_INFO
#ifdef _USE_TRACE_INFO
#define	CGTRACE_INFO								TRACE
#else
#define	CGTRACE_INFO								__noop
#endif

// 5) Info�� ����� ���� Trace
#define	_USE_TRACE_INFO_IMPORTANT
#ifdef _USE_TRACE_INFO_IMPORTANT
#define	CGTRACE_INFO_IMPORTANT						TRACE
#else
#define	CGTRACE_INFO_IMPORTANT						__noop
#endif

// 6) ���� ��Ȳ�� ����� ���� Trace
#define	_USE_TRACE_PROGRESS
#ifdef _USE_TRACE_PROGRESS
#define	CGTRACE_PROGRESS							TRACE
#else
#define	CGTRACE_PROGRESS							__noop
#endif

// 7) ����� ����� ���� Trace
#define	_USE_TRACE_USER
#ifdef _USE_TRACE_USER
#define	CGTRACE_USER								TRACE
#else
#define	CGTRACE_USER								__noop
#endif

#define	CGTRACE_IF(condition, state)					if(condition)	{ state;}

// 8) ����� ����� ���� Trace
#define	_USE_TRACE_EXCEPTION
#ifdef _USE_TRACE_EXCEPTION
#define	CGTRACE_EXCEPTION							TRACE
#else
#define	CGTRACE_EXCEPTION							__noop
#endif

// 9) ����� ����� ���� Trace
#define	_USE_TRACE_DEBUG

#ifdef _USE_TRACE_DEBUG
#define	CGTRACE_DEBUG								TRACE
#else
#define	CGTRACE_DEBUG								__noop
#endif

#ifdef _DEBUG
#define	_USE_CGNET_ASSERT
#endif

#ifdef _USE_CGNET_ASSERT
#define	CGASSERT(equation, value)						ASSERT(equation);	if(!(equation))	{ return value;}
#define	CGASSERT_TRACE(equation, value, state)			ASSERT(equation);	if(!(equation))	{ state; return value;}
#define	CGASSERT_ERROR(equation)						ASSERT(equation);
#define	CGASSERT_ERROR_TRACE(equation, state)			ASSERT(equation);	if(!(equation))	{ state;}

#define	CGASSERT_ERROR_CRITICAL							ASSERT(FALSE);
#else
#define	CGASSERT(equation, value)					
#define	CGASSERT_TRACE(equation, value, string)
#define	CGASSERT_ERROR(equation)					
#define	CGASSERT_ERROR_TRACE(equation, string)

#define	CGASSERT_ERROR_CRITICAL
#endif


// ----------------------------------------------------------------------------
// Check����
// ----------------------------------------------------------------------------
//#ifdef _DEBUG
//#define	CGNET_ASSERT_RETURN_IF(equation, value)						if(equation)			{ return value;}
//
//
//#else
//#define	CGNET_ASSERT_RETURN_IF(equation, value)						
//#endif



// 1) ��� Serise
#define	CONTINUE_IF(equation)										if(equation)			{ continue;}
#define	BREAK_IF(equation)											if(equation)			{ break;}
#define	THROW_IF(equation, value)									if(equation)			{ throw value;}
#define	RETURN_IF(equation, value)									if(equation)			{ return value;}

#define	SAFE_DELETE_AND_RETURN_IF(condition, object, value)			if(condition)			{ SAFE_DELETE(object); return (value);}
#define	DELETE_AND_RETURN_IF(condition, object, value)				if(condition)			{ delete object; return (value);}
#define	DELETE_SELF_AND_RETURN_IF(condition, value)					if(condition)			{ delete this; return (value);}


#define	GOTO_IF(equation, destination)								if(equation)			{ goto destination;}


// 2) Error Serise
#define	ERROR_CONTINUE_IF(equation, state)							if(equation)			{ state; continue;}
#define	ERROR_BREAK_IF(equation, state)								if(equation)			{ state; break;}
#define	ERROR_THROW(value, state)									{state; throw value;}
#define	ERROR_THROW_IF(equation, value, state)						if(equation)			{ state; throw value;}
#define	ERROR_RETURN_IF(equation, value, state)						if(equation)			{ state; return value;}

// 3) ��� Serise
#define	ERROR_IF(equation, state)									if(equation)			{ state;}

// 4) 
#define	HAS_ATTRIBUTE(att, ptr)										(dynamic_cast<att*>(ptr)!=NULL)



#define	CHECK_DEVICEREADY(value)									if(CCGDeviceSettingInfo::Direct3DDevice()==NULL) { TRACE("CGC2 Error : Device is not ready\n"); return value;}

// 1) Set ����
#define	SET_ZERO(variable)											variable=0
#define SET_ZERO_VECTOR3(variable)									variable.x=variable.y=variable.z=0.0f;
#define SET_ZERO_VECTOR4(variable)									variable.x=variable.y=variable.z=variable.w=0.0f;

#define	SET_NULL(variable)											variable=NULL
#define	SET_TRUE(variable)											variable=TRUE
#define	SET_FALSE(variable)											variable=FALSE


// 2) Bit���� ����
// Toggle(TRUE�� FALSE�� FALSE�� TRUE�� �ٲߴϴ�.)
#define	TOGGLE(value)												(value) = !(value)

// 3) ��...
#define	MAX(a, b)													(((a) > (b)) ? (a) : (b))
#define	MIN(a, b)													(((a) < (b)) ? (a) : (b))

template<typename TA>
void	SWAP(TA& a, TA& b) { TA t = a; a = b; b = t; }


// Ư�������� ������ �� ����մϴ�.
#define	SATURATE_MAX(value, max)									if(value > (max)) value=(max);
#define	SATURATE_MIN(value, min)									if(value < (min)) value=(min);
#define	SATURATE(value, min, max)									if(value < (min)) value=(min); else if(value > (max)) value=(max);

#define	INCREASE(value)												++value
#define	INCREASE_IF(condition, value)								if(condition) {++value;}
#define	DECREASE(value)												--value
#define	DECREASE_IF(condition, value)								if(condition) {--value;}

#define	DISCOUNT_TO_ZERO(value)										if(value > 0) {--value;}

// 4) ��� ����
// ������ ���մϴ�.
#define	SQURE(value)												(value*value)
#define	VECTOR3_DISTANCE_SQURE(value)								(value.x*value.x+value.y*value.y+value.z*value.z)
