
/**
*  ¬.ƒ.¬алединский
*  ѕростейший пример TCP клиента и сервера.
*
*  ‘айлы
*  cli_tcp.cpp  - пример TCP клиента
*  ser_tcp_select.cpp - пример TCP сервера c использованием select()
*
*   лиент в цикле получает с клавиатуры текстовую строку и отсылает
*  ее на сервер, читает и выводит на экран ответ сервера.
*  —ервер ждет соединений от клиентов. ѕри установленом
*  соединении получает строку от клиента, переводит ее в верхний
*  регистр и отсылает обратно клиенту. ≈сли клиент посылает строку
*  содержащую слово stop, то сервер закрывает соединение с этим клиентом,
*  а клиент заканчивает свою работу.
*   лиент и сервер печатают на экран протокол своей работы т.е.
*  разные данные, которые они получают или отсылают.
*
*  ƒл€ €сности примеры составлены максимально просто и не анализируют
*  некорректные ответы и запросы клиента и сервера, возможность переполнени€
*  буферов ввода вывода, неожиданное 'падение' сервера и т.п.
*
*/


#pragma warning(suppress : 4996)
#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include "windows.h"
#include <time.h>
using namespace std;
// ќпределимс€ с портом, адресом сервера и другими константами.
// ¬ данном случае берем произвольный порт и адрес обратной св€зи
// (тестируем на одной машине).
#define  SERVER_PORT     5555
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          512

// ƒве вспомогательные функции дл€ чтени€/записи (см. ниже)
int  writeToServer(int fd, int t );
int  readFromServer(int fd);
void log(const string& s);
void show(const string& s);

int  main(int argc, char** argv)
{
	const char* serverName;
	serverName = (argc < 2) ? "127.0.0.1" : argv[1];
	int err;
	int sock;
	struct sockaddr_in server_addr;
	//struct hostent    *hostinfo;

	// инициализаци€ windows sockets
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed\n");
		return -1;
	}

	// ѕолучаем информацию о сервере по его DNS имени
	// или точечной нотации IP адреса.
	//hostinfo = gethostbyname(SERVER_NAME);
	//if (hostinfo == NULL) {
	//	fprintf(stderr, "Unknown host %s.\n", SERVER_NAME);
	//	exit(EXIT_FAILURE);
	//}
	// можно было бы использовать GetAddrInfo()

	// «аполн€ем адресную структуру дл€ последующего
	// использовани€ при установлении соединени€
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//server_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr;
	unsigned int iaddr;
	inet_pton(AF_INET, serverName, &iaddr);
	server_addr.sin_addr.s_addr = iaddr;

	// —оздаем TCP сокет.
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}

	// ”станавливаем соединение с сервером
	err = connect(sock, (struct sockaddr*) & server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("Client:  connect failure");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Connection is ready\n");

	// ќбмениваемс€ данными
    cout << "HELP-spisok komand" << endl;
    string main;
    cin >> main;
    for (auto& c : main) c = toupper(c);
    while (main != "EXIT")
    {
        if (main == "HELP")
        {
            string manual;           
                ifstream help(manual);
                if (!help.is_open()) {
                    log("Help file not found: " + manual);
                }
                else {
                    string cur_line;
                    log("Help file:");
                    while (getline(help, cur_line)) {
                        show(cur_line);
                    }
                    help.close();
                }           
        }
        else if (main == "GC")
        {
                double  disp_x, disp_y, c_x, c_y;
                int dots;
                cin >> c_x >> c_y >> disp_x >> disp_y >> dots;
                if (cin.fail())
                {
                    show("Bad values");
                    show("Default cloud created");
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock,0); Sleep(1);
                    writeToServer(sock, 0); Sleep(1);
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock, 100); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    log("->oblaco sozdano s par-mi: " + to_string(1) + " " + to_string(1) + " " + to_string(0) + " " + to_string(0) + " " + to_string(100));
                    show("HELP-spisok komand");
                    cin.clear();
                }
                else
                {
                    writeToServer(sock, 1);
                    Sleep(1);
                    writeToServer(sock, c_x);
                    Sleep(1);
                    writeToServer(sock, c_y); Sleep(1);
                    writeToServer(sock, disp_x); Sleep(1);
                    writeToServer(sock, disp_y); Sleep(1);
                    writeToServer(sock, dots); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    show("oblaco sozdano");
                    log("->oblaco sozdano s par-mi" + to_string(disp_x) + " " + to_string(disp_y) + " " + to_string(c_x) + " " + to_string(c_y) + " " + to_string(dots));
                }
        }
        else if (main == "GEN")
        {
            int q;
            cin >> q;
            if (cin.fail())
            {
                show("Bad values");
                show("Default random buffer created");
                writeToServer(sock, 2); Sleep(1);
                writeToServer(sock, 500); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                log("->buffer sgenerirovan,tochek: " + to_string(500));
                cin.clear();
            }
            else
            {
                writeToServer(sock, 2); Sleep(1);
                writeToServer(sock, q); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                show("buffer sgenerirovan");
                log("buffer sgenerirovan,tochek: " + to_string(q));
            }
        }
        else if (main == "UNLOAD")
        {
        writeToServer(sock, 3); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
                show("buffer vigruzhen");;
        }
        else if (main == "MATRIX")
        {
        writeToServer(sock, 4); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
            show("matrica sozdana");
            show("pole perevedeno v sosotoyanie 2(analis)");
        }
        else if (main == "KMC")
        {
            int k, p;
            cin >> k >> p;
            if (cin.fail())
            {
                show("Bad values");
                show("KMeans with default values (3,3)");
                writeToServer(sock, 5); Sleep(1);
                writeToServer(sock, 3); Sleep(1);
                writeToServer(sock, 3); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                log("KMeans with values (3,3) printed");
                cin.clear();
            }
            else
            {
                writeToServer(sock, 5); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, p); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                show("proscanirovano(kmcore)");
                show("pole raspechatano");
                log("KMeans with values ( " + to_string(k) + "," + to_string(p) + ") printed");
            }
        }
        else if (main == "FOREL")
        {
            double r;
            int dep;
            cin >> r >> dep;
            if (dep < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Forel with default value (0.1) and depth (2) printed");
                    writeToServer(sock, 6); Sleep(1);
                    writeToServer(sock, 1); Sleep(1);
                    writeToServer(sock, 2); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    log("Forel with value (0.1) and depth (2) printed");
                    cin.clear();
                }
                writeToServer(sock, 6); Sleep(1);
                writeToServer(sock, r); Sleep(1);
                writeToServer(sock, dep); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                show("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
                log("Forel with value (" + to_string(r) + ") and depth (" + to_string(dep) + ") printed");
            }
        }
        else if (main == "KM")
        {
            int k;
            cin >> k;
            if (k < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("Kmeans with default value (5) printed");
                    writeToServer(sock, 7); Sleep(1);
                    writeToServer(sock, 5); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    log("Kmeans with value (5) printed");
                    cin.clear();
                }
                writeToServer(sock, 7); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                show("Kmeans with value (" + to_string(k) + ") printed");
                log("Kmeans with value (" + to_string(k) + ") printed");
            }
        }
        else if (main == "EM")
        {
            int k;
            cin >> k;
            if (k < 1)
            {
                show("Bad values");
                show("HELP-spisok komand");
            }
            else
            {
                if (cin.fail())
                {
                    show("Bad values");
                    show("ExpMax with default value (3) printed (gif)");
                    writeToServer(sock, 8); Sleep(1);
                    writeToServer(sock, 3); Sleep(1);
                    writeToServer(sock, 1000); Sleep(1);
                    log("ExpMax with value (5) printed");
                    cin.clear();
                }
                writeToServer(sock, 8); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                show("ExpMax with value (" + to_string(k) + ") printed (gif)");
                log("ExpMax with value (" + to_string(k) + ") printed");
            }
        }
        else if (main == "DBS")
        {

                double del;
                int k;
                cin >> del >> k;
                writeToServer(sock, 9); Sleep(1);
                writeToServer(sock, del); Sleep(1);
                writeToServer(sock, k); Sleep(1);
                writeToServer(sock, 1000); Sleep(1);
                show("proscanirovano");
                log("->proscanirovano(dbs)" + to_string(del) + to_string(k));
                    show("pole raspechatano");
                    log("->pole raspechatano (dbs) " + to_string(del) + " " + to_string(k) + " default");
        }
        else if (main == "IER")
        {
        writeToServer(sock, 10); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
            show("Hierarchy algorithm printed");
            log("Hierarchy algorithm printed");
        }
        else if (main == "VORONOI")
        {
        writeToServer(sock, 11); Sleep(1);
        writeToServer(sock, 1000); Sleep(1);
            show("Voronoi diagram printed");
            log("Voronoi diagram printed");
        }
        else if (main == "TREE")
        {
            int err;
            writeToServer(sock, 11); Sleep(1);
            writeToServer(sock, 1000); Sleep(1);
            show("derevo sozdano");
            log("->derevo sozdano s hist " + to_string(30) + " default");
                show("derevo raspechatano");
                log("->derevo  raspechatano : default");
        }
        else {
            show("neizvestnaya comanda");
            show("HELP-spisok komand");
        }
        cin >> main;
        for (auto& c : main) c = toupper(c);
    }
	//while (1) {
	//	if (writeToServer(sock) < 0) break;
	//	if (readFromServer(sock) < 0) break;
	//}
	//fprintf(stdout, "The end\n");

	// «акрываем socket
	closesocket(sock);
	WSACleanup();
	exit(EXIT_SUCCESS);
}



int  writeToServer(int fd, int t)
{
	int   nbytes;
	int n;
	char buf[BUFLEN];
	fprintf(stdout, "Send to server > ");
		sprintf(buf, "%d", t);
		if (t > 0)
		{
			n = (ceil(log10(t)) + 1);
            nbytes = send(fd, buf, n, 0);
		}
        else if (t == 0)
        {
            send(fd, "0", 1, 0);
            nbytes = 1;
        }
		else
		{
			n = (ceil(log10(abs(t))) + 1);
            nbytes = send(fd, buf, n, 0);
		}
	if (nbytes < 0) { perror("write"); return -1; }
	return 0;
}


int  readFromServer(int fd)
{
	int   nbytes;
	char  buf[BUFLEN];

	nbytes = recv(fd, buf, BUFLEN, 0);
	if (nbytes < 0) {
		// ошибка чтени€
		perror("read");
		return -1;
	}
	else if (nbytes == 0) {
		// нет данных дл€ чтени€
		fprintf(stderr, "Client: no message\n");
	}
	else {
		// ответ успешно прочитан
		fprintf(stdout, "Server's replay: %s\n", buf);
	}
	return 0;
}


void log(const string& s)
{   
    string log_file_name = "logint.txt";
    ofstream logger = ofstream(log_file_name, ios::app);
        struct tm* a;
        const time_t timer = time(NULL);
        a = localtime(&timer);
        logger << a->tm_mday << "." << (a->tm_mon + 1) << " " << (a->tm_hour) << ":" << (a->tm_min) << ":" << (a->tm_sec) << " -- " << s << endl;
        logger.close();
}
void show(const string& s)
{
    string log_file_name = "logint.txt";
    ofstream logger = ofstream(log_file_name, ios::app);
    cout << s << endl;
    log("<< " + s);
    logger.close();
}

