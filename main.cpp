/*
 * @Author: Zichu Zhao
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-08-24 14:27:22
 * @FilePath: /Assignment-1/main.cpp
 * @Description: This file contains the implementation of basic arithmetic operations (addition, subtraction, multiplication) for large integers represented as strings, using both a school method and Karatsuba algorithm.
 
 * Notes:
 * - No extra prints to stdout. Optional self-tests are disabled by default.
 * - All functions trim leading zeros and keep at least one '0'.
 * - Base is 2..10, so digits are '0'..'9' only (no A..F).
 */

# include <iostream>
# include <string>
# include <algorithm> // use reverse
# include <vector>
using namespace std;

/*
* Add two non-negative integers represented as strings in a given base using the school method.
*/
string add_school(string a, string b, int base){
    // find the max length
    int len_a = (int)a.length();
    int len_b = (int)b.length();
    int n;
    // make the lengths equal by padding with leading zeros
    if(len_a > len_b){
        n = len_a;
    } else{
         n = len_b;
    }
    // add leading zeros
    if(len_a < n){
        int diff = n - len_a;
        string temp = "";
        for(int i = 0; i < diff; i++){
            temp = temp + "0";
        }
        a = temp + a;     
    }
    // add leading zeros
    if (len_b < n){
        int diff = n - len_b;
        string temp = "";
        for(int i = 0; i < diff; i++){
            temp = temp + "0";
        }
        b = temp + b;
    }
    // perform addition from the least significant digit to the most significant digit
    int carry = 0;
    string res = "";
    for (int i = n-1; i >= 0; i--){
        int digit_a = a[i] - '0';
        int digit_b = b[i] - '0';
        int sum = digit_a + digit_b + carry;
        int digit_sum = sum % base;
        carry = sum / base;
        char ch = digit_sum + '0';
        res.push_back(ch);
    }
    // If there is leftover carry, append it
    if (carry > 0){
        char ch = carry + '0';
        res.push_back(ch);
    }

    // Result was built in reverse order -> flip it back
    reverse(res.begin(), res.end());

    // Remove leading zeros (but keep at least on digit)
    int i = 0;
    while (i < (int)res.length() - 1&& res[i] == '0'){
        i++;
    }
    string result = res.substr(i);

    return result;
}

/*
 * Subtract two non-negative integers represented as strings in a given base using the school method.
*/
string sub_school(string a, string b, int base){
    // find the max length
    int len_a = (int)a.length();
    int len_b = (int)b.length();
    int n;
    if(len_a > len_b){
        n = len_a;
    } else{
        n = len_b;
    }
    // pad with zeros
    if(len_a < n){
        int diff = n - len_a;
        string temp = "";
        for(int i = 0; i < diff; i++){
            temp = temp + "0";
        }
        a = temp + a;     
    }
    // pad with zeros
    if (len_b < n){
        int diff = n - len_b;
        string temp = "";
        for(int i = 0; i < diff; i++){
            temp = temp + "0";
        }
        b = temp + b;
    }

    // perform subtraction from the least significant digit to the most significant digit
    int borrow = 0;
    string res = "";
    for (int i = n-1; i>=0; i--){
        int digit_x = (a[i] - '0') - borrow; // apply previous borrow
        int digit_y = b[i] - '0';
        if (digit_x < digit_y){
            digit_x = digit_x + base; //borrow from next higher digit
            borrow = 1;
        }else{
            borrow = 0;
        }
        int digit_sub = digit_x - digit_y;
        char ch = digit_sub + '0';
        res.push_back(ch);
    }

    // Result was built in reverse order -> flip it back
    reverse(res.begin(), res.end());

    // Trim leading zeros
    int p = 0;
    while (p < (int)res.length() - 1 && res[p] == '0'){
        p++;
    }
    string result = res.substr(p);
    return result;
}

/*
 * Multiply two non-negative integers represented as strings in a given base using the school method.
*/

string mul_school(string a, string b, int base){
    // Trim leading zeros
    int i = 0;
    while (i < (int)a.length()-1 && a[i]== '0'){
        i++;
    }
    a = a.substr(i);

    int j = 0;
    while (j < (int)b.length()-1 && b[j] == '0'){
        j++;
    }
    b = b.substr(j);

    // If either number is zero, the product is zero
    if (a =="0" || b == "0"){
        return "0";
    }

    int acc_a = (int)a.length();
    int acc_b = (int)b.length();
    vector<int> acc(acc_a + acc_b, 0);// result can be at most acc_a + acc_b digits
    // Multiply each digit pair
    for (int ia = acc_a -1; ia >= 0; ia--){
        for (int ib = acc_b-1; ib >= 0; ib--){
            int digit_a = a[ia] - '0';
            int digit_b = b[ib] - '0';
            acc[ia + ib +1] = acc[ia + ib + 1] + digit_a * digit_b;
        }
    }
    // Handle carry
    for (int k = acc_a +acc_b -1; k >0; k--){
        acc[k-1] = acc[k-1] + acc[k] / base;
        acc[k] = acc[k] % base;
    }

    // convert result vector to string
    string res = "";
    for (int x : acc){
        res.push_back(char('0' + x));
    } 

    // Trim leading zeros
    int p = 0;
    while (p < (int)res.length() - 1 && res[p] == '0'){
        p++;
    }
    res = res.substr(p);
    return res;
     
}

/*
 * Multiply two non-negative integers represented as strings in a given base using the Karatsuba algorithm.
 * Formula: x*y = 10^(2*m)*z2 + 10^m*(z1 - z2 - z0) + z0
*/
string karatsuba(string a, string b, int base){
    // Trim leading zeros
    int i =0;
    while (i < (int)a.length() - 1 && a[i] == '0'){
        i++;
    }
    a= a.substr(i);

    int j = 0;
    while (j < (int)b.length() - 1 && b[j] == '0'){
        j++;
    }
    b = b.substr(j);

    // If either number is zero, the product is zero
    if(a == "0" || b == "0"){
        return "0";
    }

    // Base case for recursion: if numbers are small enough, use school method
    int len_a = (int)a.length();
    int len_b = (int)b.length();
    int n;
    if(len_a > len_b){
        n = len_a;
    } else {
        n = len_b;
    }
    if ( n <= 24){
        return mul_school(a, b , base);
    }

    // Make lengths equal by padding with leading zeros
    if(len_a <n){
        int diff = n - len_a;
        string temp = "";
        for(int i = 0; i < diff; i++){
            temp = temp + "0";
        }
        a = temp + a;      
    }
    if (len_b < n){
        int diff = n - len_b;
        string temp = "";
        for(int i = 0; i < diff; i++){
            temp = temp + "0";
        }
        b = temp + b;
    }

    int right = n / 2; // right part length
    int left = n - right; // left part length
    string a_left = a.substr(0, left);
    string a_right = a.substr(left);
    string b_left = b.substr(0, left);
    string b_right = b.substr(left);
    
    // Recursive multiplications
    string p1 = karatsuba(a_left, b_left, base);
    string p2 = karatsuba(a_right, b_right, base);
    string a_left_plus_right = add_school(a_left, a_right, base);
    string b_left_plus_right = add_school(b_left, b_right, base);
    string p3 = karatsuba(a_left_plus_right, b_left_plus_right, base);
    
    // p3 -(p1 + p2) = cross terms
    string p1_plus_p2 = add_school(p1,p2,base);
    string p3_minus_p1_plus_p2 = sub_school(p3, p1_plus_p2, base);

    // Combine results with appropriate powers of base (10^(2*m) and 10^m)
    if(p1 !="0"){
        string zero = "";
        for (int i =0; i<2 * right; i++){
            zero = zero + "0";
        }
        p1 = p1 + zero;
    }

    if ( p3_minus_p1_plus_p2 != "0"){
        string zero = "";
        for (int i = 0; i < right; i++){
            zero = zero + "0";
        }
        p3_minus_p1_plus_p2 = p3_minus_p1_plus_p2 + zero;
    }

    string temp = add_school(p1, p3_minus_p1_plus_p2, base);
    string result = add_school(temp, p2, base);

    // Trim leading zeros
    int p = 0;
    while (p < (int)result.length() - 1 && result[p] == '0'){
        p++;
    }
    result = result.substr(p);
    return result;

}




int main() {
    // Read input
    string I1, I2;
    int B;
    if (!(cin >> I1 >> I2 >> B)) return 0;

    // Compute results
    string sum_result = add_school(I1, I2, B);
    // string sub_result = sub_school(I1, I2, B);
    // string mul_school_result = mul_school(I1, I2, B);
    string mul_result = "0";
    string div_result = "0";
    // cout << sub_result << endl;
    // cout<< mul_school_result << endl;
    cout << sum_result << " " << mul_result << " " << div_result << endl;   
    return 0;

}