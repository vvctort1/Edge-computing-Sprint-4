import requests
import json
import matplotlib.pyplot as plt
from datetime import datetime
from datetime import timedelta

# Realize a solicitação GET como você fez antes
url = "http://46.17.108.113:8666/STH/v1/contextEntities/type/sensorAlert/id/urn:ngsi-ld:sensorAlert:9/attributes/gas?lastN=20"
headers = {
  'fiware-service': 'smart',
  'fiware-servicepath': '/'
}
response = requests.get(url, headers=headers)

# Verifique se a solicitação foi bem-sucedida
if response.status_code == 200:
    data = json.loads(response.text)

    gas_data = data['contextResponses'][0]['contextElement']['attributes'][0]['values']

    gas_values = [entry['attrValue'] for entry in gas_data]
    recv_times = [datetime.fromisoformat(entry['recvTime'][:-1]) for entry in gas_data]

    # Filtre os dados para os últimos 5 minutos
    now = datetime.now()
    last_5_minutes = now - timedelta(minutes=5)
    filtered_gas_values = []
    filtered_recv_times = []
    for i in range(len(gas_values)):
        if recv_times[i] >= last_5_minutes:
            filtered_gas_values.append(gas_values[i])
            filtered_recv_times.append(recv_times[i])

    # Crie o gráfico
    plt.plot(filtered_recv_times, filtered_gas_values)
    plt.xlabel('Tempo')
    plt.ylabel('Quantidade de Gás')
    plt.title('Últimos 40 segundos de dados de Gás')
    plt.xticks(rotation=45)
    plt.grid(True)
    plt.tight_layout()

    plt.show()
else:
    print(f"A solicitação retornou o código de status: {response.status_code}")
