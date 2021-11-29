/* ************************************************************************
> File Name:     main.cpp
> Author:        Ronnyz
> Created Time:  Wed 17 Nov 2021 09:31:28 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
using namespace std;

int main() {
    cout << "Third Party Libs Include!" << endl;

    boost::shared_ptr<int> isp(new int(4));

    boost::filesystem::path path = "usr/share/cmake/modules";
    if (path.is_relative()) {
        cout << "Path is relative." << endl;
    } else {
        cout << "Path is not relative." << endl;
    }
    
    return 0;
}
