# cpptodo

C++の勉強のために、コマンドライン上で動くTODOアプリを作りました。MySQL5.7を使います。macで作っています。

## 開発環境

- C++ (Apple LLVM version 8.1.0 (clang-802.0.42))
- MySQL5.7.18
- Mac Sierra

## インストール

### 1. MySQLでDB・テーブルを作成
MySQLでデータベースを作成し、下記tasksテーブルを作成します。

```
mysql> desc tasks;
+------------+--------------+------+-----+---------+----------------+
| Field      | Type         | Null | Key | Default | Extra          |
+------------+--------------+------+-----+---------+----------------+
| id         | int(11)      | NO   | PRI | NULL    | auto_increment |
| task       | varchar(100) | NO   |     | NULL    |                |
| status     | tinyint(1)   | NO   |     | 0       |                |
| important  | tinyint(1)   | NO   |     | 0       |                |
| sort_no    | int(6)       | YES  |     | NULL    |                |
| updated_at | datetime     | NO   |     | NULL    |                |
+------------+--------------+------+-----+---------+----------------+
```

### 2. main.cppのDB接続設定を編集

main.cppの下記を環境に合わせて編集します。

```main.cpp
#define DBHOST "localhost"
#define DBPORT 3306
#define DBUSER "root"
#define DBPASS "password"
#define DBNAME "todo"
```

### 3. makeします

コンパイルしているだけなので、todoという実行ファイルがカレントディレクトリに作成されます。

```
$ make
```

makefileはmacの環境用になっているので、環境に合わせて修正が必要になります。


## 使い方

下記はtodoをpathに追加した前提です。

todo helpでhelpが表示されます。

```
$ todo help
g <count>	show Incomplete <count> tasks
a <title>	add a <title> task
ao <title>	add a <title> task and check important
f <id>		finish the task <id>
r <id>		revert the task <id>
on <id>		check important to the <id> task
off <id>	remove check important to the <id> task
del <id>	delete the <id> task
h <count>	show completed <count> tasks 
clear		delete all completed tasks 
truncate	delete all tasks 
```

`todo`でタスク一覧が表示されます。

```
$ todo
5 *	プログラム作る
7  	Go勉強する
6  	C++勉強する
4  	歯を磨く
2  	寝る
```

`todo a`でタスクを追加します。`todo ao`でタスクの追加と同時に重要マークをつけます。

```
$ todo a "new task"
$ todo
5 *	プログラム作る
8  	new task
7  	Go勉強する
6  	C++勉強する
4  	歯を磨く
2  	寝る
```

`todo f`でタスクを完了にします。

```
$ todo f 2
$ todo
5 *	プログラム作る
8  	new task
7  	Go勉強する
6  	C++勉強する
4  	歯を磨く
```

`todo on`で重要マークがつけられます。重要なものは上に表示されます。`todo off`で重要マークを外せます。

```
$ todo on 8
$ todo
8 *	new task
5 *	プログラム作る
7  	Go勉強する
6  	C++勉強する
4  	歯を磨く
```
`todo h`で完了したタスク一覧が表示されます。

```
$ todo h
2  	寝る
1  	夕飯を食べる
3  	起きる
```

`todo d`で不要なタスクを削除できます。

```
$ todo d 3
$ todo h
2  	寝る
1  	夕飯を食べる
```

`todo r`で未完了状態に戻せます。
`todo clear`で完了済みタスクを全消去します。
`todo truncate`で、全タスクを削除し、初期化します。

## LICENSE

This software is released under the MIT License, see LICENSE.txt.