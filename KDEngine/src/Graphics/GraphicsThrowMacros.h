#pragma once

#define GFX_EXCEPT_NOINFO(hr) KDGraphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw KDGraphics::HrException( __LINE__,__FILE__,hr )

#ifdef _DEBUG
#define GFX_EXCEPT(hr) KDGraphics::HrException( __LINE__,__FILE__,(hr),m_InfoManager.Messages() )
#define GFX_THROW_INFO(hrcall) m_InfoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_THROW_INFO_ONLY(call) m_InfoManager.Set(); (call); {auto v = m_InfoManager.Messages(); if(!v.empty()) {throw KDGraphics::InfoException(__LINE__, __FILE__, v);}} 
#define GFX_EXCEPT_DEVICE_REMOVED(hr) KDGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr),m_InfoManager.Messages() )
#else
#define GFX_EXCEPT(hr) KDGraphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_THROW_INFO_ONLY(call) (call)
#define GFX_EXCEPT_DEVICE_REMOVED(hr) KDGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif