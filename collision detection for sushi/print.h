#pragma once
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <ostream>
#include <iostream>
#include <deque>
#include <list>
#include <sstream>
#include <string>



#define Lu(a,b,c,d) for(int a = b; a < c; a+= d)
#define Ld(a,b,c,d) for(int a = c; a > b; a-= d)
#define TS(x) to_string(x)

using std::cout; using std::endl;

template<class T> std::ostream& operator << (std::ostream&, std::vector<T>);
template<class T> std::ostream& operator << (std::ostream&, std::list<T>);
template<class T> std::ostream& operator << (std::ostream&, std::deque<T>);
template<class T, class U> std::ostream& operator << (std::ostream&, std::map<T, U>);
template<class T, class U> std::ostream& operator << (std::ostream&, std::unordered_map<T, U>);
template<class T> std::ostream& operator << (std::ostream&, std::set<T>);
template<class T> std::ostream& operator << (std::ostream&, std::unordered_set<T>);
template<class T, class U> std::ostream& operator << (std::ostream&, std::pair<T, U>);

template<class T>
std::ostream& operator << (std::ostream& os, std::vector<T> v) {
	os << "[";
	int x = v.size();
	int i = 0;
	for (auto& n : v) {
		
		os << n;
		if (i < x - 1) os << ", ";
		i++;
	}
	os << "]";
	return os;
}

template<class T>
std::ostream& operator << (std::ostream& os, std::list<T> v) {
	os << "[";
	int x = v.size();
	int i = 0;
	for (auto& n : v) {

		os << n;
		if (i < x - 1) os << ", ";
		i++;
	}
	os << "]";
	return os;
}

template<class T>
std::ostream& operator << (std::ostream& os, std::deque<T> v) {
	os << "[";
	int x = v.size();
	int i = 0;
	for (auto& n : v) {

		os << n;
		if (i < x - 1) os << ", ";
		i++;
	}
	os << "]";
	return os;
}

template<class T>
std::ostream& operator << (std::ostream& os, std::set<T> v) {
	os << "|";
	int x = v.size();
	int i = 0;
	for (auto& n : v) {

		os << n;
		if (i < x - 1) os << ".";
		i++;
	}
	os << "|";
	return os;
}

template<class T>
std::ostream& operator << (std::ostream& os, std::unordered_set<T> v) {
	os << "|";
	int x = v.size();
	int i = 0;
	for (auto& n : v) {
		os << n;
		if (i < x - 1) os << ".";
		i++;
	}
	os << "|";
	return os;
}



template<class T, class U>
std::ostream& operator << (std::ostream& os, std::map<T,U> v) {	
	os << "{"<< std::endl;
	int x = v.size();
	int i = 0;
	for (auto& n : v) {
		os << "" << n.first << " ::: " << n.second << "";
		if (i < x - 1) os << ", ";
		i++;
		os << std::endl;
	}
	os << "}";
	return os;
}

template<class T, class U>
std::ostream& operator << (std::ostream& os, std::unordered_map<T, U> v) {

	os << "{" << std::endl;
	int x = v.size();
	int i = 0;
	for (auto& n : v) {
		os << "" << n.first << " ::: " << n.second << "";
		if (i < x - 1) os << ", ";
		i++;
		os << std::endl;
	}
	os << "}";
	return os;
}

template<class T, class U> std::ostream& operator << (std::ostream& os, std::pair<T, U> p) {

	os << "First: " << p.first << "  Second: " << p.second << "";
	return os;
}

template<class T>
void print(T o, char end = ' ') {
	std::cout << o << end;
}

template<class T> 
void printMat(std::vector<T> v,int rowWidth, int maxtileCharacterWidth = 10) {

	cout << "Blank Values Mean Character Size to big" << endl;
	cout << "|";
	for (int i = 0; i < maxtileCharacterWidth * rowWidth; i++) cout << "-";
	cout << "|" << endl << "|";
	int j = 0;
	for (auto& t : v) {
			
		std::stringstream ss;
		ss << t;
		std::string ms = ss.str();
		int x = ms.size();
		int n = maxtileCharacterWidth - x;

		if (n >= 0) {
			cout << ms;
			for (int i = 0; i < n; i++) cout << " ";
		}
		else {
			for (int i = 0; i < maxtileCharacterWidth; i++) cout << " ";
		}
		if ((j + 1) % rowWidth == 0) {

			cout << "|" << endl<< "|";

		}
		else {
			cout << "";

		}
		j++;
		cout << "";
	}

	for (int i = 0; i < maxtileCharacterWidth * rowWidth; i++) cout << "-";
	cout << "|";

}
