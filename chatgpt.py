# This Python file uses the following encoding: utf-8
# chatgpt_api.py
import sys
import json
from openai import OpenAI
# 设置API密钥
api_key = '36702e55d7bd69c7a458dfb97ddf5c54.9rjuNBDvEnvpJK2P'


client = OpenAI(
    api_key='36702e55d7bd69c7a458dfb97ddf5c54.9rjuNBDvEnvpJK2P',
    base_url="https://open.bigmodel.cn/api/paas/v4/"
)



def call_chatgpt(prompt):
    completion = client.chat.completions.create(
        model="glm-3.5-turbo",
        messages=[
            {"role": "user", "content": prompt}
        ],
        top_p=0.7,
        temperature=0.9
     )

    return completion.choices[0].message

if __name__ == "__main__":
    print("Running in main.")
    prompt = sys.argv[1]
    result = call_chatgpt(prompt)
    print(json.dumps({"response": result}))
