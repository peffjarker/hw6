client: rsp_client.cc
	g++ -o ttt_client ttt_client.cc -lboost_system -lboost_thread -lpthread
server: rsp_server.cc
	g++ -o ttt_server ttt_server.cc -lboost_system -lboost_thread -lpthread
