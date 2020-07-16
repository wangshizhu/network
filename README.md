### 网络库概述
--------------------------------------------------

此网络库目前采用单线程、三大I/O多路复用技术（select、poll、epoll）、应用层协议暂时只支持Msgpack（即将引入Http、Pb），暂时只支持TCP服务和连接，
连接采用阻塞式连接，后续支持非阻塞式连接，可以通过命令行参数选择I/O多路复用技术和应用层协议

### 版本要求
--------------------------------------------------

使用了C++14的特性，编译器至少支持到C++14

Linux使用了CMake构建，要求CMake最低版本3.16

### Linux平台编译
--------------------------------------------------

针对网络库的使用，编写了测试项目，项目在test/test0306目录下，有对应的client，还有对应的server（test0306），clone后进入对应的目录执行build.sh即可，例如编译client：

	git clone git@github.com:wangshizhu/network.git
	
	cd ./network/test/test0306/client
	
	sh ./build.sh
	
例如编译server：
	
	git clone git@github.com:wangshizhu/network.git
	
	cd ./network/test/test0306/test0306
	
	sh ./build.sh

### Windows项目创建
--------------------------------------------------

测试类的项目应创建在test目录下，即新建项目时，项目目录应该选择到test，下面以创建test_project项目为例，展示如何将网络库引入项目中：

1. 打开VS(VS需至少支持到C++14)，选择`新建项目`，`空项目`test_project，`项目目录`选择到net/network/test

2. 打开`解决方案资源管理器`页签，右击`解决方案test_project`，选择`添加`页签，选择`现有项目`，选择common.vcxproj

3. 在`test_project`项目上右击，选择`属性`，选择`配置属性`，选择`常规`，`字符集`选择`使用Unicode字符集`

4. 在`test_project`项目上右击，`C/C++`页签下选择`常规`，在`附加包含目录`添加此目录`..\..\..\include\msgpack`

5. 在`test_project`项目上右击，在`链接器`页签下选择`常规`，在`附加库目录`添加此目录`../../../libs`，在`链接器`页签下选择`输入`，
在`附加依赖项`添加`common_d.lib、ws2_32.lib`

6. 打开`解决方案资源管理器`页签，右击`解决方案test_project`，选择`属性`页签，`通用属性`下选择`项目依赖项`，勾选common

7. 生成

### 使用测试项目做网络测试
--------------------------------------------------

测试项目test0306提供了测试方法，clone后需切换到`feature-test-noblock`分支，例如：

	git clone git@github.com:wangshizhu/network.git
	
	cd ./network
	
	git checkout feature-test-noblock
	
这个分支下提供一些测试命令行参数，例如应用于server端的`--no_accept`，更多参数使用可以通过`--help`查看
	