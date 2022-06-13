/*****************************************************************************
*  RTC engine sdk
*  Copyright (C) 2018 xueersi
*                                                                            
*  @brief    
*  Details.                                                                  
*                                                                            
*  @author   rao
*  @email    
*  @version  1.0.1(版本号)                                                  
*  @date     
*  @license  
*                                                                            
*----------------------------------------------------------------------------
*  Remark         : 
*----------------------------------------------------------------------------
*  Change History :                                                          
*  <Date>     | <Version> | <Author>        | <Description>                   
*----------------------------------------------------------------------------
*  2018/8/24  | 1.0.1   | rao                | Create file                     
*----------------------------------------------------------------------------
*                                                                            
*****************************************************************************/

#pragma once


/**@mainpage 
* @section 概述
* 为windows提供实时通讯sdk
* RTC(realtime communication)是用于开发实时通讯服务的基础工具，提供实时音视频互动功能。RTC支持多种客户端，包括windows、linux、ios、android以及web。
本文档描述RTC在windows客户端的sdk。
* @section 系统要点
*    整个sdk以RtcEngine为中心，组织管理rtc 引擎； 音、视频分别以流的方式提供外部调用
*    系统使用rtc作为名字空间
* @subsection 主体结构
*    接口放在三个头文件中：
*    1. IRTCEngine.h： 里面是IRTCEngine接口的定义以及设备管理接口的定义；
*    2. IRTCEngineEventHandler.h: 里面是IRTCEngineEventHandler接口的定义
*    3. RTCBase.h： 包括接口中使用到的各种结构、常量的定义
*    <p>IRTCEngine是整个sdk的组织者，通过这个类的接口，可以对整个Rtc 引擎总体控制。全局只有一个引擎对象。Sdk中的其他服务对象也需要从这个对象中获取。
*    Rtc中的设备管理分作视频设备管理/麦克设备管理和播放器设备管理，通过这些接口可以控制音视频设备。
*    系统中产生的事件通过IRTCEngineEventHandler接口暴露到外部，外部要接受这些事件，需要先在引擎中注册一个IRTCEngineEventHandler对象。
* 
* @subsection 线程安全
*   可以认为sdk是线程安全的，不过通过IRTCEngineEventHandler发出的时间通知，其线程来自sdk，上层需要考虑这个多线程处理方式。
* @subsection 对象安全
*    应用层在得到sdk 引擎对象的后，最后应该通过调用释放接口释放。
*    
* @section 主要工作流程
* 上层首先通过自己的业务服务器获取token，之后：
* 1. 调用initWithToken初始化sdk
* 2. 调用joinRoom进入聊天室
* 3. 功能处理：
*    3.1 预览本地视频
*    3.2 设置数据源等
*    3.3 分享自己的视频
*    。。。
* 4. 调用leaveRoom退出聊天室
* 支持动态设置相关参数，例如视频的帧率、分辨率、码率<br/>
* 支持使用yuv视频文件作为视频数据的输入
* */
