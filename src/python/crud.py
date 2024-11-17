import json
from sqlalchemy import create_engine, Column, Float, Integer
from sqlalchemy.orm import declarative_base, sessionmaker

with open('./config/config.json', 'r') as file:
    data = json.load(file)

Base = declarative_base()

class Data(Base):
    __tablename__ = 'dados'
    id = Column(Integer, primary_key=True)
    P = Column(Float)
    K = Column(Float)
    ph = Column(Float)
    umidade = Column(Float)
    temperatura = Column(Float)

engine = create_engine(data['dbengine'])
Base.metadata.create_all(engine)
Session = sessionmaker(bind=engine)
session = Session()

def criar_dados(P, K, ph, umidade, temperatura):
    novo_dado = Data(P=P, K=K, ph=ph, umidade=umidade, temperatura=temperatura)
    session.add(novo_dado)
    session.commit()
    print("Dados inseridos com sucesso!")

def ler_dados():
    dados = session.query(Data).all()
    for dado in dados:
        print(f"ID: {dado.id}, P: {dado.P}, K: {dado.K}, pH: {dado.ph}, Umidade: {dado.umidade}, Temperatura: {dado.temperatura}")

def atualizar_dados(id_dado, P, K, ph, umidade, temperatura):
    dado = session.query(Data).filter_by(id=id_dado).first()
    if dado:
        dado.P = P
        dado.K = K
        dado.ph = ph
        dado.umidade = umidade
        dado.temperatura = temperatura
        session.commit()
        print("Dados atualizados com sucesso!")
    else:
        print("Registro não encontrado.")

def deletar_dados(id_dado):
    dado = session.query(Data).filter_by(id=id_dado).first()
    if dado:
        session.delete(dado)
        session.commit()
        print("Dados deletados com sucesso!")
    else:
        print("Registro não encontrado.")

def main():
    while True:
        print("\nSelecione uma opção:")
        print("1. Criar dados")
        print("2. Ler dados")
        print("3. Atualizar dados")
        print("4. Deletar dados")
        print("5. Sair")

        opcao = input("Opção: ")

        if opcao == '1':
            P = float(input("Digite o valor de P: "))
            K = float(input("Digite o valor de K: "))
            ph = float(input("Digite o valor de pH: "))
            umidade = float(input("Digite o valor de umidade: "))
            temperatura = float(input("Digite o valor de temperatura: "))
            criar_dados(P, K, ph, umidade, temperatura)
        elif opcao == '2':
            ler_dados()
        elif opcao == '3':
            id_dado = int(input("Digite o ID do registro a ser atualizado: "))
            P = float(input("Digite o novo valor de P: "))
            K = float(input("Digite o novo valor de K: "))
            ph = float(input("Digite o novo valor de pH: "))
            umidade = float(input("Digite o novo valor de umidade: "))
            temperatura = float(input("Digite o novo valor de temperatura: "))
            atualizar_dados(id_dado, P, K, ph, umidade, temperatura)
        elif opcao == '4':
            id_dado = int(input("Digite o ID do registro a ser deletado: "))
            deletar_dados(id_dado)
        elif opcao == '5':
            print("Encerrando o programa.")
            break
        else:
            print("Opção inválida. Tente novamente.")

if __name__ == '__main__':
    main()