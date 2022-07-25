#include <iostream>
#include "image.hpp"
#include "vector.hpp"

using namespace std;
using namespace ccanny;

void test_read_write() {
    const string src = "./data/girl.jpg";
    Image img = imread(src);
    cout << "load image from " << src << endl;
    
    img.show();

    const string dst = "./data/girl_write.jpg";
    if (imwrite(img, dst)) {
        cout << "write image to " << dst << endl;
    }
}

void test_vector() {
    Vector<0> v0;
    Vector<10> v1(10);
    Vector<10> v2(v1);
    Vector<10> v3 = v2;
    Vector<10> v4 = v3 + v2;
    Vector<10> v5 = v3 - v2;
    Vector<10> v6 = v3 * v2;
    Vector<10> v7 = v3 / v2;
    cout << "v0 = " << v0 << endl
         << "v1 = " << v1 << endl
         << "v2 = " << v2 << endl
         << "v3 = " << v3 << endl
         << "v4 = " << v4 << endl
         << "v5 = " << v5 << endl
         << "v6 = " << v6 << endl
         << "v7 = " << v7 << endl;
}

int main() {
    cout << "hello canny!" << endl;
    // test_vector();
    test_read_write();
    return 0;
}