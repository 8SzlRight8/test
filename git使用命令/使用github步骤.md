# 自己上传或拷贝文件

---

## 一、建立本地连接

### 1、创建仓库

### 2、添加账户

~~~
//
git config --global user.name "8SzlRight8"

//
git config --global user.email "1767653980@qq.com"

~~~



### 3、初始化本地仓库

~~~
git init
~~~



### 4、上传文件至本地仓库

~~~
//添加到暂存区
git add .

//获取状态值
git status

//添加到GitHub仓库
git commit -m 'add a file'
~~~



## 二、建立远程连接

### 1、获取公匙 （需详细）

~~~
ssh-keygen -t rsa -b 4096 -C “1767653980@qq.com”
~~~

### 2、配置公匙

在对应目录找到公匙，绑定到github中的ssh中。

### 3、上传文件

~~~
//1）绑定远程仓库链接
git remote add origin https://github.com/8SzlRight8/test.git

	//如果链接输错了，可以使用以下命令删除链接
	git remote rm origin
	
//2）提交文件
git push -u origin master

~~~

### 4、下拉文件

~~~
//将git中的文件拉下来
git pull
~~~

# 拷贝别人的仓库

~~~
git clone http://
~~~



# 命令介绍

~~~
git clone 远程仓库地址；   拉取创建本地仓库

git pull   拉取

 git checkout .  本地所有修改的。没有的提交的，都返回到原来的状态  

git add .   不加参数默认为将修改操作的文件和未跟踪新添加的文件添加到git系统的暂存区，注意不包括删除

git add -u .    -u 表示将已跟踪文件中的修改和删除的文件添加到暂存区，不包括新增加的文件，注意这些被删除的文件被加入到暂存区再被提交并推送到服务器的版本库之后这个文件就会从git系统中消失了。

git add -A .   -A 表示将所有的已跟踪的文件的修改与删除和新增的未跟踪的文件都添加到暂存区。

git commit -m "注释"   那么git会自动将合并的结果作为备注，提交本地仓库；

git push  推送到远程仓库

git stash  先贮藏

git stash pop 再释放贮藏

git fetch 拉取最新版本

git merge dev    分支合并

git merge --abort 取消合并

git reset --hard 恢复到历史版本id 

git status  查看状态

git stash  贮藏

git stash pop 释放贮藏

git reset --soft HEAD^ 或git reset --soft HEAD~1     回撤最后一次commit 

git branch  查看本地分支

git branch -r 查看远程分支

git branch -a 查看所有的分支

git branch dev  创建本地分支

git push origin dev  本地分支push到远程

git push origin --delete dev  删除远程分支

git branch -D dev  删除本地分支

~~~





# 问题汇总

~~~
1、git push 后面加上 -u origin master 是什么意思？加不加有什么区别？
	使用-u 就是告诉git后面的origin master 参数。一旦输入一次，后面就不需要输入参数了，git自动记住了。
~~~

