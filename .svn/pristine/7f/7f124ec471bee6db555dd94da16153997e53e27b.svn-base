_jsonp_begin = r'jsonClosureCallback('
_jsonp_end = r');'
import json
import requests

url = "http://www1.toronto.ca/transportation/roadrestrictions/RoadRestrictions.json"

def from_jsonp(jsonp_str):
    jsonp_str = jsonp_str.strip()

    if not jsonp_str.startswith(_jsonp_begin) or \
            not jsonp_str.endswith(_jsonp_end):
        raise ValueError('Invalid JSONP')

    return json.loads(jsonp_str[len(_jsonp_begin):-len(_jsonp_end)])

def return_json_dict():
    web_response = requests.get(url)
    if web_response.status_code != 200:
        raise ValueError('Bad connection')

    raw_result = web_response.content

    json_dict = (from_jsonp(raw_result))["Closure"]

    return json_dict


