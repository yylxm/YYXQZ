# -*- coding: utf-8 -*-
from sys import argv
import urllib
import re

def call_http(method, req_url, req_data=''):
    timeout_seconds = 30
    try_times = 3
    count = 0
    while (count < try_times):
        try:
            if method == 'get':
                req = urllib.request.Request(req_url)
            elif method == 'post':
                data_urlencode = urllib.urlencode(req_data)
                req = urllib.request.Request(url=req_url, data=data_urlencode)
            else:
                print('error method, return')
                return
            response = urllib.request.urlopen(url=req, timeout=timeout_seconds)
        except urllib.request.URLError as e:
            if hasattr(e, 'reason'):
                print('URLError, Reason: %s', e.reason)
            elif hasattr(e, 'code'):
                print('URLError, Code: ', e.code)
            count += 1
            continue
        else:
            try:
                res = response.read()
            except Exception as e:
                print('Socket Error: ', e)
                return
            else:
                return res
def get_price(response):


def price_parse(output_file):
    array = []
    array_line_1 = ["0101", "0102", "0103", "0104", "0105", "0106", "0107", "0108", "0109", "0110",
                  "0111", "0112", "0113","0114", "0115", "0116", "0117", "0118", "0119", "0120",
                  "0121", "0122", "0123", "0124", "0125", "0126", "0127", "0128", "0129", "0130"]
    array_line_2 = ["0201", "0202", "0203", "0204", "0205", "0206", "0207", "0208", "0209", "0210",
                  "0211", "0212", "0213","0214", "0215", "0216", "0217", "0218", "0219", "0220",
                  "0221", "0222", "0223", "0224", "0225", "0226", "0227", "0228", "0229"]
    array_line_3 = ["0301", "0302", "0303", "0304", "0305", "0306", "0307", "0308", "0309", "0310",
                    "0311", "0312", "0313", "0314", "0315", "0316", "0317", "0318", "0319", "0320",
                    "0321", "0322", "0323", "0324", "0325", "0326", "0327", "0328", "0329", "0330"]
    array_line_4 = ["0401", "0402", "0403", "0404", "0405", "0406", "0407", "0408", "0409", "0410",
                    "0411", "0412", "0413", "0414", "0415"]
    array_line_5 = ["0501", "0502", "0503", "0504", "0505", "0506", "0507", "0508", "0509", "0510",
                    "0511", "0512", "0513", "0514", "0515", "0516", "0517", "0518", "0519", "0520",
                    "0521", "0522", "0523", "0524", "0525", "0526", "0527"]
    array_line_11 = ["1101", "1102", "1103", "1104", "1105", "1106", "1107", "1108", "1109", "1110",
                    "1111", "1112", "1113", "1114", "1115", "1116", "1117", "1118"]

    array.append(array_line_1)
    array.append(array_line_2)
    array.append(array_line_3)
    array.append(array_line_4)
    array.append(array_line_5)
    array.append(array_line_11)

    with open(output_file, 'w') as f:
        for startline in array:
            for startstop in startline:
                for endline in array:
                    for endstop in endline:
                        url = "http://www.szmc.net/szmc/minxl?format=html&startcode="
                        url = url + startstop + "&endcode=" + endstop + "&_=1470649792610"
                        get_url = call_http('get', url)
                        p =
                        print(get_url)
                        f.write(str(p) + "\t")

if __name__ == '__main__':

    price_parse(argv[1])

