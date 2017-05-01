#include <iostream>
#include <mysql/mysql.h>
#include <boost/format.hpp>

#define DBHOST "localhost"
#define DBPORT 3306
#define DBUSER "root"
#define DBPASS "password"
#define DBNAME "todo"

using std::string;

void help()
{
	std::cout << "g <count>\tshow Incomplete <count> tasks\n";
	std::cout << "a <title>\tadd a <title> task\n";
	std::cout << "ao <title>\tadd a <title> task and check important\n";
	std::cout << "f <id>\t\tfinish the task <id>\n";
	std::cout << "r <id>\t\trevert the task <id>\n";
	std::cout << "on <id>\t\tcheck important to the <id> task\n";
	std::cout << "off <id>\tremove check important to the <id> task\n";
	std::cout << "del <id>\tdelete the <id> task\n";
	std::cout << "h <count>\tshow completed <count> tasks \n";
	std::cout << "clear\t\tdelete all completed tasks \n";
	std::cout << "truncate\tdelete all tasks \n";
}

class mysql
{
  protected:
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	void execute(string sql);

  public:
	mysql();
	~mysql();
};

mysql::mysql()
{
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, DBHOST, DBUSER, DBPASS, DBNAME, DBPORT, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

mysql::~mysql()
{
	mysql_free_result(res);
	mysql_close(conn);
}

void mysql::execute(string sql)
{
	if (mysql_query(conn, sql.c_str())) //todo sanitaize?
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);
}

class todo : public mysql
{
	void check_id(int id);

  public:
	void get(int limit, string mode);
	void add(string task, string mode);
	void fin(int id, string mode);
	void star(int id, string mode);
	void del(int id);
	void clear();
	void truncate();
};

void todo::check_id(int id)
{
	if (!id)
	{
		std::cerr << "invalid id\n";
		exit(1);
	}
}

void todo::get(int limit = 20, string mode = "get")
{
	string sql = "select * from tasks where status = %1%";
	sql += " order by important desc,updated_at desc limit %2%";
	limit = !limit ? 20 : limit;
	int status = mode == "his" ? 1 : 0;
	sql = (boost::format(sql) % status % limit).str();
	execute(sql);
	string star;
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		star = atoi(row[3]) ? " *" : "  ";
		std::cout << row[0] << star << "\t" << row[1] << std::endl;
	}
}

void todo::add(string task, string mode = "add")
{
	if (task == "")
	{
		std::cerr << "invalid task\n";
		exit(1);
	}
	int star = mode == "add" ? 0 : 1;
	string sql = "insert into tasks";
	sql += " (task, important, updated_at) values ('%1%', %2%, now())";
	sql = (boost::format(sql) % task % star).str();
	execute(sql);
}

void todo::del(int id)
{
	check_id(id);
	string sql = "delete from tasks where id = %1%";
	sql = (boost::format(sql) % id).str();
	execute(sql);
}

void todo::fin(int id, string mode = "fin")
{
	check_id(id);
	int status = mode == "rev" ? 0 : 1;
	string sql = "update tasks set status = %1%, important = 0,";
	sql += " updated_at = now() where id = %2%";
	sql = (boost::format(sql) % status % id).str();
	execute(sql);
}

void todo::star(int id, string mode = "on")
{
	check_id(id);
	int status = mode == "on" ? 1 : 0;
	string sql = "update tasks set important = %1%,";
	sql += " updated_at = now() where id = %2%";
	sql = (boost::format(sql) % status % id).str();
	execute(sql);
}

void todo::clear()
{
	execute("delete from tasks where status = 1");
}

void todo::truncate()
{
	execute("truncate table tasks");
}

int check_int(std::string str)
{
	try
	{
		return stoi(str);
	}
	catch (...)
	{
		return 0;
	}
}

void action(todo *td, string mode, string param)
{
	if (mode == "g")
	{
		td->get(check_int(param));
	}
	else if (mode == "a")
	{
		td->add(param);
	}
	else if (mode == "ao")
	{
		td->add(param, "star");
	}
	else if (mode == "d")
	{
		td->del(check_int(param));
	}
	else if (mode == "f")
	{
		td->fin(check_int(param));
	}
	else if (mode == "r")
	{
		td->fin(check_int(param), "rev");
	}
	else if (mode == "h")
	{
		td->get(check_int(param), "his");
	}
	else if (mode == "on")
	{
		td->star(check_int(param));
	}
	else if (mode == "off")
	{
		td->star(check_int(param), "off");
	}
	else if (mode == "clear")
	{
		td->clear();
	}
	else if (mode == "truncate")
	{
		td->truncate();
	}
	else
	{
		help();
	}
}

int main(int argc, char *argv[])
{
	todo td;
	if (argc < 2)
	{
		td.get();
	}
	else
	{
		action(&td, argv[1], argc < 3 ? "" : argv[2]);
	}
	return 0;
}