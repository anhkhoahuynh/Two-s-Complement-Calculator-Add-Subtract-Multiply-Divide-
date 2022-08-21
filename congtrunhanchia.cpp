#include <iostream>
#include <string>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

struct ketquacong
{
	int finalsum[9];
	wstring overflow;
};

struct ketquacong17bit
{
	int finalsum[18];
	wstring overflow;
};

struct ketquanhan
{
	wstring tichcodau;
	wstring tichkhongdau;
};

struct ketquachia
{
	wstring thuong;
	wstring sodu;
};

bool checkbinum(wstring binum)
{
	bool checkbinum;
	for (int i = 0; i < binum.length(); i++)
	{
		if ((binum[i] != '1' && binum[i] != '0') || binum.length() != 8)
		{
			checkbinum = false;
			break;
		}
		else
		{
			checkbinum = true;
		}
	}
	return checkbinum;
}

ketquacong cong(wstring num1, wstring num2)
{
	int i = 0, sodu = 0;
	ketquacong ketqua;
	int sum[9];
	if (num1.length() == 9) //nếu có 9 số thì loại bỏ bit đầu tiên
	{
		num1 = num1.substr(1, 8);
	}
	if (num2.length() == 9)//nếu có 9 số thì loại bỏ bit đầu tiên
	{
		num2 = num2.substr(1, 8);
	}
	int numc1 = stoi(num1);
	int numc2 = stoi(num2);
	while (numc1 != 0 || numc2 != 0)
	{
		sum[i++] = (numc1 % 10 + numc2 % 10 + sodu) % 2;
		sodu = (numc1 % 10 + numc2 % 10 + sodu) / 2;
		numc1 = numc1 / 10;
		numc2 = numc2 / 10;
	}
	sum[i++] = sodu;
	while (i <= 8)
	{
		sum[i++] = 0; //thêm 0 vào đầu để standardize kết quả luôn đủ 9 số
	}
	for (int i = 0; i < 9; i++)
	{
		ketqua.finalsum[i] = sum[8 - i];
	}//phải ra 00101010
	if (num1[0] == num2[0])
	{
		wchar_t daucuasum = wchar_t(ketqua.finalsum[1])+48;//Cong 48 de chuyen thanh 1
		if (daucuasum != num1[0])
		{
			ketqua.overflow = L"yes";
		}
		else
		{
			ketqua.overflow = L"no";
		}
	}
	else
	{
		ketqua.overflow = L"no";
	}
	return ketqua; //lúc nào cũng ra 9 số
}

ketquacong17bit cong17bitvoi1(wstring num1,wstring num2) //Dùng để chuyển đổi kết quả nhân không dấu thành kết quả nhân có dấu
{
	int i = 0, sodu = 0;
	ketquacong17bit ketqua;
	int sum[18];
	unsigned long long numc1 = stoll(num1);
	int numc2 = stoi(num2);
	while (numc1 != 0 || numc2 != 0)
	{
		sum[i++] = (numc1 % 10 + numc2 % 10 + sodu) % 2;
		sodu = (numc1 % 10 + numc2 % 10 + sodu) / 2;
		numc1 = numc1 / 10;
		numc2 = numc2 / 10;
	}
	if (sodu != 1)
	{
		sum[i++] = sodu;
	}
	for (int i = 0; i < 18; i++)
	{
		ketqua.finalsum[i] = sum[17 - i];
	}
	if (num1[0] == num2[0])
	{
		wchar_t daucuasum = wchar_t(ketqua.finalsum[1]) + 48;//Cong 48 de chuyen thanh 1
		if (daucuasum != num1[0])
		{
			ketqua.overflow = L"yes";
		}
		else
		{
			ketqua.overflow = L"no";
		}
	}
	else
	{
		ketqua.overflow = L"no";
	}
	return ketqua; //lúc nào cũng ra 18 số
}

wstring chuyensobitruthanham(wstring sobitru)//Dùng để chuyển đổi dấu của số trừ
{
	wstring sobitrudangam, sobitrudoi;
	sobitrudoi = sobitru;
	for (int i = 0; i < sobitru.length(); i++)
	{
		if (sobitru[i] == L'0')
		{
			sobitrudoi[i] = L'1';
		}
		else
		{
			sobitrudoi[i] = L'0';
		}
	}
	ketquacong tongcuasobitrudoiva1 = cong(sobitrudoi, L"00000001");
	if (tongcuasobitrudoiva1.overflow == L"yes")
	{
		sobitrudangam = L"overflow khi chuyển dấu";
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			if (tongcuasobitrudoiva1.finalsum[i] != 0 && tongcuasobitrudoiva1.finalsum[i] != 1)
			{
				continue;
			}
			sobitrudangam += to_wstring(tongcuasobitrudoiva1.finalsum[i]);
		}
	}
	return sobitrudangam;// ra 9 số
}

ketquacong tru(wstring num1, wstring num2)//Dùng để trừ 2 dãy bit, sử dụng kết quả từ phép chuyển đổi dấu của số trừ
{
	ketquacong hieu;
	if (num1.length() == 9) //nếu có 9 số thì loại bỏ bit đầu tiên
	{
		num1 = num1.substr(1, 8);
	}
	if (num2.length() == 9)//nếu có 9 số thì loại bỏ bit đầu tiên
	{
		num2 = num2.substr(1, 8);
	}
	if (num2 == L"10000000")//Vì -128 là con số đặc biệt vì phạm vi giới hạn của 8 bit nên không thể chuyển -128 thành +128 được (xảy ra hiện tượng tràn số. Nếu gặp trường hợp này thì sẽ cộng thêm vào số trừ và số bị trừ 1 đơn vị
	{
		wstring num1cong1;
		ketquacong tongnum1cong1 = cong(num1, L"00000001");
		for (int i = 0; i < 9; i++)
		{
			num1cong1 += to_wstring(tongnum1cong1.finalsum[i]);
		}
		num2 = L"10000001";
		hieu = cong(num1cong1, chuyensobitruthanham(num2));
	}
	else
	{
		hieu = cong(num1, chuyensobitruthanham(num2));
	}
	return hieu;
}

wstring nhankhongdau(wstring M, wstring Q)//Dùng để nhân không dấu
{
	if (M.length() == 9) //nếu có 9 số thì loại bỏ bit đầu tiên
	{
		M = M.substr(1, 8);
	}
	if (Q.length() == 9)//nếu có 9 số thì loại bỏ bit đầu tiên
	{
		Q = Q.substr(1, 8);
	}
	wstring C = L"0";
	wstring A = L"00000000";
	wstring product = C+A + Q;
	for (int i = Q.length()-1; i >= 0; i--)
	{
		if (product[product.length() - 1] == '1')
		{
			ketquacong tongAM = cong(A, M);
			wstring Anew;
			for (int i = 0; i < 9; i++)
			{
				Anew += to_wstring(tongAM.finalsum[i]);
			}
			product = Anew + Q;
			product.insert(0, L"0");
			product.pop_back();
		}
		else
		{
			product.insert(0, L"0");
			product.pop_back();
		}
		A = product.substr(1, 8);
		Q = product.substr(9, 8);
	}
	return product;
}

wstring timsokhongdau(wstring num)//Chuyển 2 dãy bit thành số không dấu trước khi tiến hành nhân không dấu
{
	wstring sodoi;
	if (num[0] == L'1')
	{
		ketquacong phepchuyen = tru(L"00000000", num);
		for (int i = 1; i < 9; i++)
		{
			sodoi += to_wstring(phepchuyen.finalsum[i]);
		}
	}
	else
	{
		sodoi = num;
	}
	return sodoi;
}

long long bi2dec(wstring binum)//Dùng để tính giá trị thập phân của dãy bit, không xét dấu
{
	long long kq = 0;
	for (int i = 0; i < binum.length(); i++)
	{
		int kytu = binum[i] - '0';
		kq += kytu * pow(2, binum.length() - 1 - i);
	}
	return kq;
}

wstring sothapphan(wstring num)//Tính giá trị thập phân của dãy 8 bit, có xét dấu
{
	if (num.length() == 9)
	{
		for (int i = 1; i < num.length(); i++)
		{
			num.substr(1, 8);
		}
	}
	wstring sothapphan = to_wstring(bi2dec(timsokhongdau(num)));
	if (num[0] == L'1')
	{
		return L'-' + sothapphan;
	}
	else
	{
		return L'+' + sothapphan;
	}
}

wstring sothapphan17bit(wstring num1, wstring num2, wstring kqnhankhongdau)//Dùng để hiện kết quả nhân có dấu ở dạng thập phân để tiện theo dõi và kiểm tra
{
	if ((num1[0] == L'1' && num2[0] == L'1') || (num1[0] == L'0' && num2[0] == L'0'))
	{
		return L'+' + to_wstring(bi2dec(kqnhankhongdau));
	}
	if ((num1[0] == L'0' && num2[0] == L'1') || (num1[0] == L'1' && num2[0] == L'0'))
	{
		return L'-' + to_wstring(bi2dec(kqnhankhongdau));
	}
}

wstring chuyenkhongdauthanhsoamcodau(wstring num)//Dùng để chuyển đổi kết quả nhân không dấu thành kết quả nhân có dấu trong trường hợp nhân 2 số trái dấu
{
	wstring num1s, num2s;
	for (int i = 0; i < num.length(); i++)
	{
		if (num[i] == L'0')
		{
			num1s += L'1';
		}
		else
		{
			num1s += L'0';
		}
	}
	ketquacong17bit kq = cong17bitvoi1(num1s, L"00000000000000001");
	kq.overflow = L"";
	for (int i = 0; i < 18; i++)
	{
		if (kq.finalsum[i] == 1 || kq.finalsum[i] == 0)
		{
			num2s += to_wstring(kq.finalsum[i]);
		}
	}
	if (num2s.length() != 17)
	{
		num2s = num2s.substr(1, 17);
	}
	return num2s;
}

ketquanhan nhansocodau(wstring num1, wstring num2)//Dùng để nhân có dấu, tùy theo dấu của 2 dãy bit mà sẽ phân chia thành các trường hợp cụ thể
{
	ketquanhan kqnhan;
	if (num1[0] == L'1' && num2[0] == L'1')
	{
		wstring num1khongdau = timsokhongdau(num1);
		wstring num2khongdau = timsokhongdau(num2);
		kqnhan.tichkhongdau = kqnhan.tichcodau = nhankhongdau(num1khongdau, num2khongdau);
	}
	if (num1[0] == L'0' && num2[0] == L'0')
	{
		kqnhan.tichkhongdau = kqnhan.tichcodau = nhankhongdau(num1, num2);
	}
	if (num1[0] == L'0' && num2[0] == L'1')
	{
		wstring num2khongdau = timsokhongdau(num2);
		kqnhan.tichkhongdau = nhankhongdau(num1, num2khongdau);
		kqnhan.tichcodau = chuyenkhongdauthanhsoamcodau(kqnhan.tichkhongdau);
	}
	if (num1[0] == L'1' && num2[0] == L'0')
	{
		wstring num1khongdau = timsokhongdau(num1);
		kqnhan.tichkhongdau = nhankhongdau(num1khongdau, num2);
		kqnhan.tichcodau = chuyenkhongdauthanhsoamcodau(kqnhan.tichkhongdau);
		
	}
	return kqnhan;
}

ketquachia chiakhongdau(wstring num1, wstring num2)//Dùng để chia không dấu
{
	wstring A = L"00000000";
	wstring product = A + num1;
	wstring Q = num1;
	wstring M = num2;
	ketquachia kqc;
	for (int i = 0; i < 8; i++)
	{
		product.insert(16, L"0");
		product.erase(0, 1);
		A = product.substr(0, 8);
		Q = product.substr(8, 8);
		ketquacong AtruM = tru(A, M);
		wstring Anew;
		for (int i = 1; i < 9; i++)
		{
			Anew += to_wstring(AtruM.finalsum[i]);
		}
		if (Anew[0] == L'1')
		{
			product = A + Q;
			product[product.length() - 1] = L'0';
		}
		else
		{
			product = Anew + Q;
			product[product.length() - 1] = L'1';
		}
	}
	A = product.substr(0, 8);
	Q = product.substr(8, 8);
	kqc.thuong = Q;
	kqc.sodu = A;
	return kqc;
}

ketquachia chiacodau(wstring num1, wstring num2)//Dùng để chia có dấu, dùng kết quả từ method chia không dấu kết hợp với thao tác xử lý dấu của kết quả trong từng trường hợp cụ thể
{
	wstring num1khongdau = timsokhongdau(num1);
	wstring num2khongdau = timsokhongdau(num2);
	ketquachia kqkhongdau = chiakhongdau(num1khongdau, num2khongdau);
	wstring thuong = kqkhongdau.thuong;
	wstring sodu = kqkhongdau.sodu;
	if (num1[0] != num2[0])
	{
		thuong = chuyensobitruthanham(thuong);
		thuong = thuong.substr(1, 8);
	}
	if (num1[0] == L'1')
	{
		sodu = chuyensobitruthanham(sodu);
		sodu = sodu.substr(1, 8);
	}
	ketquachia kqcodau;
	kqcodau.thuong = thuong;
	kqcodau.sodu = sodu;
	return kqcodau;
}

void thehienketquacong(ketquacong kq)
{
	wstring xuatkqcong;
	if (kq.overflow == L"no")
	{
		for (int i = 1; i < 9; i++) //Chọn i=1 để chỉ xuất ra chỉ 8 ký tự trong TH không bị overflow cho nên dù kết quả có 9 ký tự
		{
			xuatkqcong += to_wstring(kq.finalsum[i]);
		}
		wcout << L"-> Tổng 2 dãy bit trên là: " << xuatkqcong << L" (" << sothapphan(xuatkqcong) << L" ở hệ 10)" << endl;
	}
	else
	{
		wcout << L"-> Đã xảy ra hiện tượng tràn số. Dãy bit thu được là: ";
		for (int i = 0; i < 9; i++)
		{
			if (kq.finalsum[i] != 0 && kq.finalsum[i] != 1) //Lồng điều kiện để đảm bảo không xuất ra màn hình ký tự lạ dư thừa trong TH finalsum chỉ có 8 ký tự mà vẫn bị overflow
			{
				continue;
			}
			wcout << kq.finalsum[i];
		}
	}
}

void thehienketquatru(ketquacong kq)
{
	wstring xuatkqtru;
	if (kq.overflow == L"no")
	{
		for (int i = 1; i < 9; i++) //Chọn i=1 để chỉ xuất ra chỉ 8 ký tự trong TH không bị overflow cho nên dù kết quả có 9 ký tự
		{
			xuatkqtru += to_wstring(kq.finalsum[i]);
		}
		wcout << L"-> Hiệu 2 dãy bit trên là: " << xuatkqtru << L" (" << sothapphan(xuatkqtru) << L" ở hệ 10)" << endl;
	}
	else
	{
		wcout << L"-> Đã xảy ra hiện tượng tràn số. Dãy bit thu được là: ";
		for (int i = 0; i < 9; i++)
		{
			if (kq.finalsum[i] != 0 && kq.finalsum[i] != 1) //Lồng điều kiện để đảm bảo không xuất ra màn hình ký tự lạ dư thừa trong TH finalsum chỉ có 8 ký tự mà vẫn bị overflow
			{
				continue;
			}
			wcout << kq.finalsum[i];
		}
	}
}

void menu()
{
	wstring str;
	wcout << L"----*----*----*----\nNhập số tương ứng với từng loại phép tính cho 2 số nhị phân 8 bit (số nguyên có dấu dạng bù 2):\n[1] Tổng\n[2] Hiệu\n[3] Tích\n[4] Thương\n----*----*----*----\nNhập số bạn chọn: ";
	wcin >> str;
	while (str != L"1" && str != L"2" && str != L"3" && str != L"4")
	{
		wcin.clear();
		wcin.ignore();
		wcout << L"Chỉ được phép nhập ký tự 1-4 tương ứng với từng loại chuyển đổi.\nVui lòng nhập lại: ";
		wcin >> str;
	}
	int choice = stoi(str);
	switch (choice)
	{
	case 1:
	{
		wcout << L"Bạn đã chọn phép tính tổng.\nHãy nhập số thứ nhất: ";
		wstring strnum1;
		unsigned int num1;
		wcin >> strnum1;
		while (1)
		{
			if (checkbinum(strnum1) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ nhất: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum1;
			}
			else
			{
				wcout << L"Số thứ nhất là: " << strnum1 << L" (" << sothapphan(strnum1) << L" ở hệ 10)" << endl;
				break;
			}
		}

		wcout << L"Hãy nhập số thứ hai: ";
		wstring strnum2;
		unsigned int num2;
		wcin >> strnum2;
		while (1)
		{
			if (checkbinum(strnum2) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ hai: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum2;
			}
			else
			{
				wcout << L"Số thứ hai là: " << strnum2 << L" (" << sothapphan(strnum2) << L" ở hệ 10)" << endl;
				break;
			}
		}
		ketquacong result1 = cong(strnum1, strnum2);
		thehienketquacong(result1);
	}
	break;
	case 2:
	{
		wcout << L"Bạn đã chọn phép tính hiệu.\nHãy nhập số thứ nhất: ";
		wstring strnum1;
		unsigned int num1;
		wcin >> strnum1;
		while (1)
		{
			if (checkbinum(strnum1) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ nhất: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum1;
			}
			else
			{
				wcout << L"Số thứ nhất là: " << strnum1 << L" (" << sothapphan(strnum1) << L" ở hệ 10)" << endl;
				break;
			}
		}
		wcout << L"Hãy nhập số thứ hai: ";
		wstring strnum2;
		unsigned int num2;
		wcin >> strnum2;
		while (1)
		{
			if (checkbinum(strnum2) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ hai: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum2;
			}
			else
			{
				wcout << L"Số thứ hai là: " << strnum2 << L" (" << sothapphan(strnum2) << L" ở hệ 10)" << endl;
				break;
			}
		}
		ketquacong result2 = tru(strnum1, strnum2);
		thehienketquatru(result2);
	}
	break;
	case 3:
	{
		wcout << L"Bạn đã chọn phép tính tích.\nHãy nhập số thứ nhất: ";
		wstring strnum1;
		unsigned int num1;
		wcin >> strnum1;
		while (1)
		{
			if (checkbinum(strnum1) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ nhất: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum1;
			}
			else
			{
				wcout << L"Số thứ nhất là: " << strnum1 << L" (" << sothapphan(strnum1) << L" ở hệ 10)" << endl;
				break;
			}
		}
		wcout << L"Hãy nhập số thứ hai: ";
		wstring strnum2;
		unsigned int num2;
		wcin >> strnum2;
		while (1)
		{
			if (checkbinum(strnum2) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ hai: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum2;
			}
			else
			{
				wcout << L"Số thứ hai là: " << strnum2 << L" (" << sothapphan(strnum2) << L" ở hệ 10)" << endl;
				break;
			}
		}
		ketquanhan result3 = nhansocodau(strnum1, strnum2);
		wcout << L"-> Tích 2 dãy bit trên là: " << result3.tichcodau << L" (" << sothapphan17bit(strnum1, strnum2, result3.tichkhongdau) << L" ở hệ 10)" << endl;
	}
	break;
	case 4:
	{
		wcout << L"Bạn đã chọn phép tính thương.\nHãy nhập số thứ nhất: ";
		wstring strnum1;
		unsigned int num1;
		wcin >> strnum1;
		while (1)
		{
			if (checkbinum(strnum1) == false)
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân, chỉ được phép chứa ký tự 1-0 và có độ dài 8 bit.\nVui lòng nhập lại số thứ nhất: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum1;
			}
			else
			{
				wcout << L"Số thứ nhất là: " << strnum1 << L" (" << sothapphan(strnum1) << L" ở hệ 10)" << endl;
				break;
			}
		}
		wcout << L"Hãy nhập số thứ hai: ";
		wstring strnum2;
		unsigned int num2;
		wcin >> strnum2;
		while (1)
		{
			if (checkbinum(strnum2) == false || strnum2 == L"00000000" )
			{
				wcout << L"Bạn đã nhập sai định dạng số nhị phân hoặc nhập số chia bằng 0, chỉ được phép chứa ký tự 1-0, có độ dài 8 bit và khác \"00000000\". \nVui lòng nhập lại số thứ nhất: ";
				wcin.clear();
				wcin.ignore();
				wcin >> strnum2;
			}
			else
			{
				wcout << L"Số thứ hai là: " << strnum2 << L" (" << sothapphan(strnum2) << L" ở hệ 10)" << endl;
				break;
			}
		}
		ketquachia result4 = chiacodau(strnum1, strnum2);
		wcout << L"-> Thương 2 dãy bit trên là: " << result4.thuong << L" (" << sothapphan(result4.thuong) << L" ở hệ 10) và số dư là: " << result4.sodu << L" (" << sothapphan(result4.sodu) << L" ở hệ 10)" << endl;
	}
	break;
	}
}

int main()
{
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	wstring tieptuc;
	wcout << L"Môn Hệ Thống Máy Tính - Phần khảo sát số nguyên\nTên SV: Huỳnh Anh Khoa\nMSSV: 21880228\nBài tập 2" << endl;
	do {
		menu();
		wcout << L"\n-------------------\nBạn muốn tiếp tục hay kết thúc?\n[0] Tiếp tục\n[Nút khác bất kỳ] Thoát\n-------------------" << std::endl;
		wcin >> tieptuc;
	} 
	while (tieptuc == L"0");
}