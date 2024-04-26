# 一、创建仓库

# 二、添加账户

~~~
//
git config --global user.name "8SzlRight8"

//
git config --global user.email "1767653980@qq.com"

~~~



# 三、初始化本地仓库

# 四、进行上传等操作

~~~
//添加到暂存区
git add .

//或取状态值
git status

//添加到GitHub仓库
git commit -m 'add a file'
~~~



# 五、建立远程连接

## 1、获取公匙

~~~
ssh-keygen -t rsa -b 4096 -C “1767653980@qq.com”
~~~

## 2、配置公匙

在对应目录找到公匙，绑定到github中的ssh中。

## 3、上传文件

~~~
//1）绑定远程仓库链接
git remote add origin https://github.com/8SzlRight8/test.git

	//如果链接输错了，可以使用以下命令删除链接
	git remote rm origin
	
//2）提交文件
git push -u origin master

~~~

