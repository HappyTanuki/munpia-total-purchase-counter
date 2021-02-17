#include "FIO.h"

int main() {
	FILE*		fp;
	char*		Files[100];
	wchar_t*	buff = NULL;
	wchar_t*	Fnd = NULL;
	int			FC_location = 0, cnt = 0, i, j;
	int			PAYCO = 0, CREDIT_CARD = 0, IOS = 0, CULTURE_GIFT_CARD = 0, NAVER_PAY = 0, PAYPAL = 0, T_MONEY = 0, UNKNOWN = 0;
	char		Payment_Method[20] = "";

	setlocale(0, "ko-KR");

	FC_location = Find_File(Files, "*.html");

	for (i = 0; i < FC_location; i++) {
		if (fopen_s(&fp, Files[i], "r,ccs=UTF-8") != 0) {
			printf("아니 파일 왜 없음? 분명 실행할 땐 있었음 ㅋㅋㄹㅃㅃ\n");
			break;
		}

		while (!feof(fp)) {
			buff = (wchar_t*)calloc(1, sizeof(wchar_t));
			F_Get_Line(fp, &buff);

			if (wcsstr(buff, L"<tr bgcolor=\"#FFFFFF\">") != NULL || wcsstr(buff, L"<tr bgcolor=\"#eef7ff\">") != NULL) {
				for (j = 0; j < 4; j++) {
					buff = (wchar_t*)calloc(1, sizeof(wchar_t));
					F_Get_Line(fp, &buff);
				}
				Fnd = wcsstr(buff, L"</td>");
				*Fnd = L'\0';

				if (!wcscmp(buff, L"PAYCO"))
					strcpy_s(Payment_Method, 20, "PAYCO");
				else if (!wcscmp(buff, L"신용카드"))
					strcpy_s(Payment_Method, 20, "CREDIT_CARD");
				else if (!wcscmp(buff, L"IOS"))
					strcpy_s(Payment_Method, 20, "IOS");
				else if (!wcscmp(buff, L"문화상품권"))
					strcpy_s(Payment_Method, 20, "CULTURE_GIFT_CARD");
				else if (!wcscmp(buff, L"네이버페이"))
					strcpy_s(Payment_Method, 20, "NAVER_PAY");
				else if (!wcscmp(buff, L"페이팔 (해외결제)"))
					strcpy_s(Payment_Method, 20, "PAYPAL");
				else if (!wcscmp(buff, L"티머니"))
					strcpy_s(Payment_Method, 20, "T_MONEY");
				else
					strcpy_s(Payment_Method, 20, "UNKNOWN");

				for (j = 0; j < 2; j++) {
					buff = (wchar_t*)calloc(1, sizeof(wchar_t));
					F_Get_Line(fp, &buff);
				}
				Fnd = wcsstr(buff, L" 원");
				*Fnd = L'\0';

				Del_Comma(buff);

				if (!strcmp(Payment_Method, "PAYCO"))
					PAYCO += _wtoi(buff);
				else if (!strcmp(Payment_Method, "CREDIT_CARD"))
					CREDIT_CARD += _wtoi(buff);
				else if (!strcmp(Payment_Method, "IOS"))
					IOS += _wtoi(buff);
				else if (!strcmp(Payment_Method, "CULTURE_GIFT_CARD"))
					CULTURE_GIFT_CARD += _wtoi(buff);
				else if (!strcmp(Payment_Method, "NAVER_PAY"))
					NAVER_PAY += _wtoi(buff);
				else if (!strcmp(Payment_Method, "PAYPAL"))
					PAYPAL += _wtoi(buff);
				else if (!strcmp(Payment_Method, "T_MONEY"))
					T_MONEY += _wtoi(buff);
				else
					UNKNOWN += _wtoi(buff);
			}
		}
	}

	printf("PAYCO            : %d\n", PAYCO);
	printf("신용카드         : %d\n", CREDIT_CARD);
	printf("IOS              : %d\n", IOS);
	printf("문화상품권       : %d\n", CULTURE_GIFT_CARD);
	printf("네이버페이       : %d\n", NAVER_PAY);
	printf("페이팔 (해외결제): %d\n", PAYPAL);
	printf("티머니           : %d\n", T_MONEY);
	printf("이외의 결제수단  : %d\n", UNKNOWN);
	printf("총합             : %d\n", PAYCO + CREDIT_CARD + IOS + CULTURE_GIFT_CARD + NAVER_PAY + PAYPAL + T_MONEY + UNKNOWN);

	return 0;
}