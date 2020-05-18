### Windows项目创建
--------------------------------------------------

测试类的项目应创建在test目录下，即新建项目时，项目目录应该选择到test，下面以创建test_project项目为例，展示如何将网络库引入项目中：

1. 打开VS(VS需至少支持到C++14)，选择<font color="#dd0000">新建项目</font>，空项目test_project，项目目录选择到net/network/test

2. 打开解决方案资源管理器页签，右击解决方案test_project，选择添加页签，选择现有项目，选择common.vcxproj

3. 在test_project项目上右击，选择配置属性，选择常规，字符集选择使用Unicode字符集

4. 在test_project项目上右击，C/C++页签下选择常规，在附加包含目录添加此目录..\..\..\include\msgpack

5. 在test_project项目上右击，在链接器页签下选择常规，在附加库目录添加此目录../../../libs，在链接器页签下选择输入，
在附加依赖项添加common_d.lib、ws2_32.lib

6. 打开解决方案资源管理器页签，右击解决方案test_project，选择属性页签，通用属性下选择项目依赖项，勾选common

7. 生成