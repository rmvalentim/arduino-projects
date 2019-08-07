<?php

	$parametro="";
	
	if(isset($_GET['L']))
		$parametro.= "L=" . $_GET['L'];
	
	// URL para onde serс enviada a requisiчуo GET
	$url_feed = "192.168.0.178?" . $parametro;
	 
	// Inicia a sessуo cURL
	$ch = curl_init();
	 
	// Informa a URL onde serс enviada a requisiчуo
	curl_setopt($ch, CURLOPT_URL, $url_feed);
	 
	// Se true retorna o conteњdo em forma de string para uma variсvel
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	 
	// Envia a requisiчуo
	$result = curl_exec($ch);
	 
	// Finaliza a sessуo
	curl_close($ch);
	
	//Capturando retorno do Arduino em Json e convertendo para Array
	$retorno_arduino = json_decode($result, true);
		
	//Montando o parтmetro com os dados do array 
	$novo_parametro = "umid=" . $retorno_arduino["umid"] . "&temp=" . $retorno_arduino["temp"];
	
	// Redirecionando para o formulario
	header("Location: formulario.php?" . $novo_parametro);
?>