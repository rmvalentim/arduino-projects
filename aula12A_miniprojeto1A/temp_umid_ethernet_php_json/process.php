<?php

	$parametro="";
	
	if(isset($_GET['L']))
		$parametro.= "L=" . $_GET['L'];
	
	// URL para onde ser� enviada a requisi��o GET
	$url_feed = "192.168.0.178?" . $parametro;
	 
	// Inicia a sess�o cURL
	$ch = curl_init();
	 
	// Informa a URL onde ser� enviada a requisi��o
	curl_setopt($ch, CURLOPT_URL, $url_feed);
	 
	// Se true retorna o conte�do em forma de string para uma vari�vel
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	 
	// Envia a requisi��o
	$result = curl_exec($ch);
	 
	// Finaliza a sess�o
	curl_close($ch);
	
	//Capturando retorno do Arduino em Json e convertendo para Array
	$retorno_arduino = json_decode($result, true);
		
	//Montando o par�metro com os dados do array 
	$novo_parametro = "umid=" . $retorno_arduino["umid"] . "&temp=" . $retorno_arduino["temp"];
	
	// Redirecionando para o formulario
	header("Location: formulario.php?" . $novo_parametro);
?>