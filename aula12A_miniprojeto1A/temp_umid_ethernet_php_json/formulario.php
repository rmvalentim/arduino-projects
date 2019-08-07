<?php
	if(isset($_GET['temp']))
		$temp = $_GET['temp'];
	else
		$temp = "";
	
	if(isset($_GET['umid']))
		$umid = $_GET['umid'];
	else
		$umid = "";
?>
<!DOCTYPE HTML>
<html>
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <title>Consultar Valores de Temperatura e Umidade</title>        
		<style>
			.redText {
				color: red;
			}
		</style>
    </head>
    <body>
	<h1>Consultar Valores de Temperatura e Umidade</h1>
    <form name="formulario" id="formulario_temp_umid" action="process.php" method="get">
		<input type="hidden" id="L" name="L" value="1" />
		<div>
			<h2>Temperatura: <span class="redText"><?php if($temp == "") { echo ("Sem Medição"); } else { echo ($temp . " ºC"); } ?></span> </h2>
	   	</div>
		<div>
			<h2>Umidade: <span class="redText"><?php if($umid == "") { echo ("Sem Medição"); } else { echo ($umid . " %"); } ?></span> </h2>
	   	</div>
		<div><input type="submit" name="consultar" value="Consultar" /></div>
    </form>
</html>