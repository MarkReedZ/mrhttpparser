/*
 * Copyright (c) 2013-2018 Mark Reed
 *                         
 */

#include <assert.h>
#include <stdio.h>
#include "mrhttpparser.h"

#define RESP  \
    "HTTP/1.1 200 OK\r\n" \
    "Date: Mon, 26 Mar 2018 00:02:24 GMT\r\n" \
    "Expires: -1\r\n" \
    "Cache-Control: private, max-age=0\r\n" \
    "Content-Type: text/html; charset=ISO-8859-1\r\n" \
    "P3P: CP=\"This is not a P3P policy! See g.co/p3phelp for more info.\"\r\n" \
    "Server: gws\r\n" \
    "X-XSS-Protection: 1; mode=block\r\n" \
    "X-Frame-Options: SAMEORIGIN\r\n" \
    "Set-Cookie: 1P_JAR=2018-03-26-00; expires=Wed, 25-Apr-2018 00:02:24 GMT; path=/; domain=.google.com\r\n" \
    "Set-Cookie: NID=124=Z97v03W8SlN8OWD0zzlC8d88SM3CBPdl43WccK_QvIo_dCoNr1zrq7ReiajPreu-5ggaSMMKS6Lr0rdxIiTQMYRkA79usnU9AyNdlpeInFWAA8WAGZkKtcamVsHfZr_D; expires=Tue, 25-Sep-2018 00:02:24 GMT; path=/; domain=.google.com; HttpOnly\r\n" \
    "Transfer-Encoding: chunked\r\n" \
    "Accept-Ranges: none\r\n" \
    "Vary: Accept-Encoding\r\n" 

#define POSTREQ \
    "POST /joyent/http-parser HTTP/1.1\r\n" \
    "Host: github.com\r\n" \
    "DNT: 1\r\n" \
    "Accept-Encoding: gzip, deflate, sdch\r\n" \
    "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n" \
    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) " \
        "AppleWebKit/537.36 (KHTML, like Gecko) " \
        "Chrome/39.0.2171.65 Safari/537.36\r\n" \
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" \
    "Referer: https;//github.com/joyent/http-parser\r\n" \
    "Connection: keep-alive\r\n" \
    "Transfer-Encoding: chunked\r\n" \
    "Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n"

#define REQ1                                                                                                                        \
    "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\n"                                                \
    "Host: www.kittyhell.com\r\n"                                                                                                  \
    "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 "             \
    "Pathtraq/0.9\r\n"                                                                                                             \
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"                                                  \
    "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n"                                                                                 \
    "Accept-Encoding: gzip,deflate\r\n"                                                                                            \
    "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n"                                                                            \
    "Referer: https;//github.com/joyent/http-parser\r\n" \
    "Keep-Alive: 115\r\n"                                                                                                          \
    "Connection: keep-alive\r\n"                                                                                                   \
    "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; "                                                          \
    "__utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; "                                                             \
    "__utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"             \
    "\r\n"

#define REQ2 \
  "GET /MarkReedZ/MrJSON HTTP/1.1\r\n" \
  "Host: github.com\r\n" \
  "Connection: keep-alive\r\n" \
  "Upgrade-Insecure-Requests: 1\r\n" \
  "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36\r\n" \
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n" \
  "Accept-Encoding: gzip, deflate, br\r\n" \
  "Accept-Language: en-US,en;q=0.9\r\n" \
  "Cookie: last_write=9392597662310--zAhJdToJVGltZQ0ghhyAULskpAo6DW5hbm9fbnVtaQJ8AjoNbmFub19kZW5pBjoNc3VibWljcm8iB2NgOgtvZmZzZXRp%2FoCPOgl6b25lSSIIUFNUBjoGRVQ%3D--b761d36723502350717cef0d7d2e4858732264a2; __utmc=1; _octo=MH1.1.407662308.3472312943; tz=America%2FNew_York; user_session=zwHPFVsweWHORd6SuaeYcBEhqRW94NEIU0njSybiET7niLkO; __Host-user_session_same_site=ZwHPFVsweWHO234RF6SuaeYcBEhqRWo4NEIU0njSybiET7niLkO; logged_in=yes; dotcom_user=MarkReedZ; _gh_sess=R2I1bjhqdDZ0THNKckJqd3FlcFlRbjNJRUpiU084dU9UbVF4ZC81ZUsxVEgzMmhwSnBkWGR3TExzbmxuWnpoNDNkc1RUVXMr3kJmQ2svVHZBRkxZYkFUeXJIaTNraVV5YVM4anpCWnp4VG1XeD41d1J3Vm92U3laMkVXa1Nta2wySzFzUTZHNFVGMWtsN21Xc3FIQ1hERWF0RllNUUxpSFh6OUZJWS9TUzFzZjRCTjljQ1F3STgvYTA1SHc4O29QOGpEdzC2Y1hMbjlYbEtDU2pQOFJUZCEWmdrTUJtL3hpa3lPb244QVpHN1VjREpIL3pQcjhjZ2xTbVk4cXptODlVNlViWk1EY1l5dmVrQ1BrS2tRRmJYNTkvTjREOGVoV25aNmNBWCtreFM3MWs9LS1uZjBdfslJU1grUEsrOS9EaFpwaFRnPT0%3D--91cb95044f6813e14f1a4420826e27626c8e6666\r\n" \
    "\r\n"

#define REQ3 \
  "GET /r/python/ HTTP/1.1\r\n" \
  "Host: www.reddit.com\r\n" \
  "Connection: keep-alive\r\n" \
  "Cache-Control: max-age=0\r\n" \
  "Upgrade-Insecure-Requests: 1\r\n" \
  "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36\r\n" \
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n" \
  "Accept-Encoding: gzip, deflate, br\r\n" \
  "Accept-Language: en-US,en;q=0.9\r\n" \
  "Cookie: aaaaaaa_recentclicks2=83jfZt9kaa%2C83jfZtbroq%2C83jfZt31c8%2C83jfZt5ds0%2C83jfZt5775; bbbbbbbbb_recentclicks2=83jfZ9dusf%2C83jfZ9ef47%2C83jfZ9e84v%2C83jfZ9e0wm%2C83jfZ9bvs2; __utmc=197362089; ccccccccc_recentclicks2=83jfZlo4tx%2C83jfZlpmql%2C83jfZlplss%2C83jfZlobl3%2C83jfZlpqbw; zzzzzzz_recentclicks2=83jfZsmkhy%2C83jfZsoiom%2C83jfZsk04d%2C83jfZsj7oj%2C83jfZsjm2z; __utmc=55650728; dddddddddd_recentclicks2=83jfZy4kwp%2C83jfZy505s%2C83jfZy5f8a%2C83jfZy5tkn%2C83jfZy6cff; zzzzzfdasfdsaffdas=83jfZz2cl1%2C83jfZz0wrm%2C83jfZz0ruq%2C83jfZz1ggh%2C83jfZz160j; fdsafewfsdfdasf=83jfZc5h0v%2C83jfZc5phb%2C83jfZc65hg%2C83jfZc4oj3%2C83jfZc4x61; fdsafwefwafewae=83jfZrbv3m%2C83jfZrcnql%2C83jfZrd0b2%2C83jfZrcrsl%2C83jfZrd842; _recent_srs=Zs238unr%2CZs22u3ds%2CZs22vcl0%2CZs22qt55%2CZs22qh1s%2CZs22ty1q%2CZs22r090%2CZs23an0q%2CZs22qmij%2CZsZs2f0a; token.sig=712321312fdsafuuQKyzts; loid=1033330004000.2.913610123217.Z0FBQsacUFQHZDc4NnV4RHNielI0STNY82w1X9d2dmJudzU1SFJ0WnhXYzVKWXVWNU8tcGVKeFBGNkZfVjZaTzFQVEUwWEJzdnBrSGJ4UXUzUS1zQjA5ZFppUE1WdXJaejVGMDdqZmN3cXM4UzQwLXN2Yi1kU2lyTHZGNGFWRMVXF0ZlNKdWI; edgebucket=12LeK69QJpCbldsa9rDu; _recent_srs=Zs238unr%2CZs22u3ds%2CZs22vcl0%2CZs22qt55%2CZs22qh1s%2CZs22ty1q%2CZs22r090%2CZs23an0q%2CZs22qmij%2CZs22tf0a; reddit_session=99938527%2C2014-07-12T19%3A45%3A10%2Cda9e9b918e91ed9bccdecc946324d28bd42634950640c; pc=ab; reddaid=TAACADW3Z2WF5G7L3ZQ1; rseor=; rseor2=; rseor3=; aa=1; rabt=; token=823948392eyJAfdsnNFDEZlbiI6InJEeEowaUJCVGotewrf89afDAFS32QktHSSIsInRva2VuVHlwZSI6ImJlYXJlciIsImV4cGlyZXMiOiIyMDE4LTA0LTAyVDEzOjI2OjU4Ljk4OVoiLCJyZWZyZXNoVG9rZW4iOiI0MTIzODUyNy1fVHNwM2xDVjUyUEJZaDhwVjVDVzBXM0kxT0kiLCJzY29wZSI6ImFjY291bnQgY3JlZGRpdHMgZWRpdCBmbGFpciBoaXN0b3J5IGlkZW50aXR5IGxpdmVtYW5hZ2UgbW9kY29uZmlnIG1vZGNvbnRyaWJ1dG9ycyBtb2RmbGFpciBtb2Rsb2cgbW9kbWFpbCBtb2RvdGhlcnMgbW9kcG9zdHMgbW9kc2VsZiBtb2R3aWtpIG15c3VicmVkZGl0cyBwcml2YXRlbWVzc2FnZXMgcmVhZCByZXBvcnQgc2F2ZSBzdHJ1Y3R1cmVkc3R5bGVzIHN1Ym1pdCBzdWJzY3JpYmUgdm90ZSB3aWtpZWRpdCB3aWtpcmVhZCJ9.1; fdsadfdasfdsafasdfds=z3_b8wmjc%2C83jfZby5a7%2C83jfZbcdvx%2C83jfZbcdr1%2C83jfZbcd1n; wpsn=true; fdasfdasfdsafas=Zs238unr%2CZs22vcl0%2CZs238gfm%2CZs22u3ds%2CZs235xig%2CZs232vl0%2CZs22qt55%2CZs23cct3%2CZs22tf0a%2CZs22qh3l; session_tracker=9213u13128uoyUu0f7kt.a.93123123522.Z0FBafdasf3238234llMFDSFamF1Q0Z1V3d0TF9vRFDSMFrewfef4rf4RFDSAFEWFfdsafDSgERVrGdsfdSGtrHdfgHtrdHtSFgdfGSDefdsfas0dnNERlBkWjFDSAFd432gbhotrhgfHGfdhogf2HGfohDFGDFfd1gjfsDGdfsgsSDFgMzEMzJ6THFMNXpPZkFFa1p3MV9mbU0wNFo\r\n" \
    "\r\n"

#define REQ4 \
  "GET / HTTP/1.1\r\n" \
  "Host: localhost:8080\r\n" \
  "Cookie: last_write=9392597662310--zAhJdToJVGltZQ0ghhyAULskpAo6DW5hbm9fbnVtaQJ8AjoNbmFub19kZW5pBjoNc3VibWljcm8iB2NgOgtvZmZzZXRp%2FoCPOgl6b25lSSIIUFNUBjoGRVQ%3D--b761d36723502350717cef0d7d2e4858732264a2; __utmc=1; _octo=MH1.1.407662308.3472312943; tz=America%2FNew_York; user_session=zwHPFVsweWHORd6SuaeYcBEhqRW94NEIU0njSybiET7niLkO; __Host-user_session_same_site=ZwHPFVsweWHO234RF6SuaeYcBEhqRWo4NEIU0njSybiET7niLkO; logged_in=yes; dotcom_user=MarkReedZ; _gh_sess=R2I1bjhqdDZ0THNKckJqd3FlcFlRbjNJRUpiU084dU9UbVF4ZC81ZUsxVEgzMmhwSnBkWGR3TExzbmxuWnpoNDNkc1RUVXMr3kJmQ2svVHZBRkxZYkFUeXJIaTNraVV5YVM4anpCWnp4VG1XeD41d1J3Vm92U3laMkVXa1Nta2wySzFzUTZHNFVGMWtsN21Xc3FIQ1hERWF0RllNUUxpSFh6OUZJWS9TUzFzZjRCTjljQ1F3STgvYTA1SHc4O29QOGpEdzC2Y1hMbjlYbEtDU2pQOFJUZCEWmdrTUJtL3hpa3lPb244QVpHN1VjREpIL3pQcjhjZ2xTbVk4cXptODlVNlViWk1EY1l5dmVrQ1BrS2tRRmJYNTkvTjREOGVoV25aNmNBWCtreFM3MWs9LS1uZjBdfslJU1grUEsrOS9EaFpwaFRnPT0%3D--91cb95044f6813e14f1a4420826e27626c8e6666\r\n" \
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n" \
  "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n" \
  "Accept-Encoding: gzip,deflate\r\n" \
  "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n" \
  "Referer: https;//github.com/joyent/http-parser\r\n" \
  "\r\n"

int main(void)
{
  const char *method, *path;
  size_t method_len, path_len, num_headers;
  struct mr_header headers[64];
  int ret, i, status, minor_version;
  int lens[5];
  lens[0] = sizeof(REQ1) - 1;
  lens[1] = sizeof(REQ2) - 1;
  lens[2] = sizeof(REQ3) - 1;
  lens[3] = sizeof(REQ4) - 1;
  num_headers = sizeof(headers) / sizeof(headers[0]);

  if (1) {
    for (i = 0; i < 100000; i++) {
      ret = mr_parse_request(REQ1, lens[0], &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
      ret = mr_parse_request(REQ2, lens[1], &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
      ret = mr_parse_request(REQ3, lens[2], &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
      ret = mr_parse_request(REQ4, lens[3], &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    }
    return 0;
  }
  //for (i = 0; i < 1; i++) {
    //num_headers = sizeof(headers) / sizeof(headers[0]);
    //ret = mr_parse_response(RESP, sizeof(RESP) - 1, &minor_version, &status, &path, &path_len, headers, &num_headers, 0);
  //}


  return 0;
}
