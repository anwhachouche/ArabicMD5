ArabicMD5
=========

ArabicMD5 é um projeto implementado utilizando a comunicação entre um ou vários Arduinos conectados a um servidor Java, utilizando conexão via Socket. O servidor Java ao ser iniciado na porta pré-definida, pede que usuário informe a quantidade de clientes (Arduinos) e em seguida pede uma senha Criptografada em MD5. Quando um cliente encontrar a senha, informa ao Servidor. Por padrão a senha deve ser de no máximo três caracteres, essa configuração foi definida pelo tempo tempo gasto para encontrar a senha.