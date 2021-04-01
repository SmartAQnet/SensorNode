var searchData=
[
  ['abovenn',['abovenn',['../main_8h.html#a5fa85ffee34300f181ee595ca35caac9',1,'main.h']]],
  ['abovennestimated',['abovennEstimated',['../main_8h.html#abdc4960daecc2ccc289d9a50f11ccb47',1,'main.h']]],
  ['ac_5fdbg',['AC_DBG',['../my_auto_connect_8h.html#a1d9ab114fc74078065bc8e794e94e8b0',1,'myAutoConnect.h']]],
  ['ac_5fdbg_5fdumb',['AC_DBG_DUMB',['../my_auto_connect_8h.html#a6457491905e867b69dcb0a77b0a069f9',1,'myAutoConnect.h']]],
  ['ac_5fdebug',['AC_DEBUG',['../my_auto_connect_8h.html#aeb1a0199d34a44c8d37bc8c366a1eccf',1,'myAutoConnect.h']]],
  ['ac_5fdebug_5fport',['AC_DEBUG_PORT',['../my_auto_connect_8h.html#a621f7534c11ca3ed064df74db88adbe9',1,'myAutoConnect.h']]],
  ['acelement',['ACElement',['../main_8h.html#a1c6b64234ad0c2c95a942e59aa06b966',1,'ACElement(cfgScript, &quot;  &lt;script src=\&quot;http://www.openlayers.org/api/OpenLayers.js\&quot;&gt;&lt;/script&gt;\n&quot; &quot;  &lt;script&gt;\n&quot; &quot;    //global vars\n&quot; &quot;    var latitude;\n&quot; &quot;    var longitude;\n&quot; &quot;    //var elevation;\n&quot; &quot;    var address            = \&quot;\&quot;;\n&quot; &quot;    var wgsProjection      = new OpenLayers.Projection(\&quot;EPSG:4326\&quot;);   // WGS 1984 Projection\n&quot; &quot;    var mercatorProjection = new OpenLayers.Projection(\&quot;EPSG:900913\&quot;); // Spherical Mercator Projection\n&quot; &quot;    var initialposition    = new OpenLayers.LonLat(8.424171,49.013034).transform(wgsProjection, mercatorProjection);\n&quot; &quot;    var initialzoom        = 3;\n&quot; &quot;    var position           = initialposition;\n&quot; &quot;    var zoom               = 18;\n&quot; &quot;    var mapnik             = new OpenLayers.Layer.OSM();\n&quot; &quot;    var markers            = new OpenLayers.Layer.Markers(\&quot;Markers\&quot;);\n&quot; &quot;    var marker             = new OpenLayers.Marker(position);\n&quot; &quot;    var touchNavigation    = new OpenLayers.Control.TouchNavigation({\n&quot; &quot;                                 defaultClickOptions:{\n&quot; &quot;                                     pixelTolerance: 10\n&quot; &quot;                                 }\n&quot; &quot;                             });\n&quot; &quot;\n  &quot; &quot;    // function to geocode: look up coordinates from address text field, populate lon and lat fields and update map \n&quot; &quot;    function geolookup() {\n&quot; &quot;      address = document.getElementById(&apos;inputAddress&apos;).value;\n&quot; &quot;      var request = new XMLHttpRequest();\n&quot; &quot;      var url = \&quot;https://nominatim.openstreetmap.org/search/&apos;\&quot; + encodeURI(address) + \&quot;&apos;?format=json&amp;limit=1\&quot;;\n&quot; &quot;      request.open(&apos;GET&apos;, url, true);\n&quot; &quot;      request.onload = function() {\n&quot; &quot;        if (this.status &gt;= 200 &amp;&amp; this.status &lt; 300) {\n&quot; &quot;          var response = JSON.parse(this.responseText);\n&quot; &quot;          if (response[0]) {\n&quot; &quot;            latitude = Number(response[0].lat).toFixed(6);\n&quot; &quot;            longitude = Number(response[0].lon).toFixed(6);\n&quot; &quot;            document.getElementById(\&quot;inputLon\&quot;).value = longitude;\n&quot; &quot;            document.getElementById(\&quot;inputLat\&quot;).value = latitude;\n&quot; &quot;            updateMarker();\n&quot; &quot;          } else {\n&quot; &quot;            alert(\&quot;Keine Koordinaten gefunden, bitte Adress-Anfrage ändern.\&quot;);\n&quot; &quot;          }\n&quot; &quot;        } \n&quot; &quot;      };\n&quot; &quot;      request.send();\n&quot; &quot;    }\n&quot; &quot;\n  &quot; &quot;    // function to \&quot;move\&quot; marker (avoiding marker.moveTo(), as it is an \&quot;unofficial\&quot; function)\n&quot; &quot;    function replaceMarker() {\n&quot; &quot;      markers.clearMarkers();\n&quot; &quot;      position = new OpenLayers.LonLat(longitude,latitude).transform(wgsProjection, mercatorProjection);\n&quot; &quot;      marker = new OpenLayers.Marker(position);\n&quot; &quot;      markers.addMarker(marker);\n&quot; &quot;    }\n&quot; &quot;\n  &quot; &quot;\n  &quot; &quot;    // function to update marker based on inputs and center on map\n&quot; &quot;    function updateMarker() {\n&quot; &quot;      longitude = document.getElementById(\&quot;inputLon\&quot;).value;\n&quot; &quot;      latitude = document.getElementById(\&quot;inputLat\&quot;).value;  \n&quot; &quot;\n  &quot; &quot;      if (typeof map !== &apos;undefined&apos;) {\n&quot; &quot;        if (latitude &amp;&amp; longitude) {\n&quot; &quot;          replaceMarker();\n&quot; &quot;          map.setCenter(position, zoom);\n&quot; &quot;        } else {\n&quot; &quot;          markers.clearMarkers();\n&quot; &quot;          map.setCenter(initialposition, initialzoom);\n&quot; &quot;        }\n&quot; &quot;      }\n&quot; &quot;    }\n&quot; &quot;\n  &quot; &quot;    // function to \&quot;move\&quot; marker on click in map\n&quot; &quot;    OpenLayers.Util.extend(touchNavigation, {\n&quot; &quot;      defaultClick: function(evnt) {\n&quot; &quot;        var lonlat = map.getLonLatFromViewPortPx(evnt.xy);\n&quot; &quot;\n&quot; &quot;        latitude =  lonlat.transform(mercatorProjection, wgsProjection).lat.toFixed(6);\n&quot; &quot;        longitude = lonlat.lon.toFixed(6);\n&quot; &quot;\n&quot; &quot;        document.getElementById(\&quot;inputLon\&quot;).value = longitude;\n&quot; &quot;        document.getElementById(\&quot;inputLat\&quot;).value = latitude;\n&quot; &quot;\n&quot; &quot;        replaceMarker();\n&quot; &quot;      }\n&quot; &quot;    });\n&quot; &quot;\n&quot; &quot;    // function to unfocus current form element when enter key was pressed\n&quot; &quot;    function unfocusForm() {\n&quot; &quot;      if (event.key == &apos;Enter&apos;) {\n&quot; &quot;        document.activeElement.blur();\n&quot; &quot;        return false;\n&quot; &quot;      } else {\n&quot; &quot;        return true;\n&quot; &quot;      }\n&quot; &quot;    }\n&quot; &quot;\n  &quot; &quot;    // execute the following once all DOM elements are loaded\n&quot; &quot;    document.addEventListener(&apos;DOMContentLoaded&apos;, function(){\n&quot; &quot;\n  &quot; &quot;        // add geocoding button\n&quot; &quot;        var findbutton = document.createElement(\&quot;button\&quot;);\n&quot; &quot;        findbutton.setAttribute(\&quot;id\&quot;, \&quot;geocode\&quot;);\n&quot; &quot;        findbutton.setAttribute(\&quot;type\&quot;, \&quot;button\&quot;);\n&quot; &quot;        findbutton.innerHTML = \&quot;Suche Koordinaten\&quot;;\n&quot; &quot;        findbutton.addEventListener (\&quot;click\&quot;, function() {\n&quot; &quot;          geolookup();\n&quot; &quot;        });\n&quot; &quot;        document.getElementById(\&quot;pgeocode\&quot;).appendChild(findbutton);\n&quot; &quot;\n  &quot; &quot;        // init OpenLayers map\n&quot; &quot;        map = new OpenLayers.Map(\&quot;mapDiv\&quot;);\n&quot; &quot;        map.addLayer(mapnik);\n&quot; &quot;        map.addLayer(markers);\n&quot; &quot;\n  &quot; &quot;        updateMarker();\n&quot; &quot;\n  &quot; &quot;        // register function to \&quot;move\&quot; marker\n&quot; &quot;        map.addControl(touchNavigation);   \n&quot; &quot;        touchNavigation.activate();\n&quot; &quot;\n  &quot; &quot;        document.getElementById(\&quot;inputLon\&quot;).addEventListener(\&quot;change\&quot;, updateMarker);\n&quot; &quot;        document.getElementById(\&quot;inputLat\&quot;).addEventListener(\&quot;change\&quot;, updateMarker);\n&quot; &quot;\n  &quot; &quot;\n  &quot; &quot;    });\n&quot; &quot;  &lt;/script&gt;\n&quot;):&#160;main.h'],['../main_8h.html#ad14516a9a717a2b7a3b3c882f2b0a2a6',1,'ACElement(cfgBody, &quot;&lt;style type=\&quot;text/css\&quot;&gt; \n&quot; &quot;  input[type=number], input#owner { font-weight: 300; width: calc(100% - 124px); background-color: #fff; border: 1px solid #ccc; border-radius: 2px; color: #444; margin: 8px 0 8px 0; padding: 10px; }\n&quot; &quot;  button[type=button] { font-weight: normal; padding: 8px 14px; margin: 12px; width: auto; outline: none; text-decoration: none; background-color: #1b5e20; border-color: #1b5e20; letter-spacing: 0.8px; color: #fff; border: 1px solid; border-radius: 2px; font-size: 0.9em; }\n&quot; &quot;  div#OpenLayers_Control_Attribution_16 { bottom: .2em; }\n&quot; &quot;&lt;/style&gt;\n&quot; &quot;\n&quot; &quot;  &lt;div id=\&quot;mapDiv\&quot; style=\&quot;height:250px\&quot;&gt;&lt;br/&gt;&lt;/div&gt;&lt;br/&gt;\n&quot; &quot;\n&quot; &quot;  &lt;h3&gt;Standort des Sensors&lt;/h3&gt;\n&quot; &quot;  &lt;label for=\&quot;inputAddress\&quot;&gt;Adresse, Format: Straße Hausnummer, Stadt &lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;input type=\&quot;text\&quot; name=\&quot;inputAddress\&quot; id=\&quot;inputAddress\&quot; maxlength=\&quot;160\&quot; placeholder=\&quot;Adresse\&quot; value=\&quot;\&quot;  style=\&quot;width: calc(100% - 124px);\&quot;&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;span id=\&quot;pgeocode\&quot;&gt;&lt;/span&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;label for=\&quot;inputLon\&quot;&gt;&lt;b&gt;Länge&lt;/b&gt;, Format: -3.703790 (für Europa im Bereich -15 bis 35):&lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;input type=\&quot;number\&quot; name=\&quot;inputLon\&quot; id=\&quot;inputLon\&quot; min=\&quot;-180\&quot; max=\&quot;180\&quot; step=\&quot;0.000001\&quot; placeholder=\&quot;z.B. -3.703790\&quot; value=\&quot;\&quot; required onchange=\&quot;updateMarker()\&quot;&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;label for=\&quot;inputLat\&quot;&gt;&lt;b&gt;Breite&lt;/b&gt;, Format: 40.416775 (für Europa im Bereich 35 bis 75):&lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;input type=\&quot;number\&quot; name=\&quot;inputLat\&quot; id=\&quot;inputLat\&quot; min=\&quot;-90\&quot; max=\&quot;90\&quot; step=\&quot;0.000001\&quot; placeholder=\&quot;z.B. 40.416775\&quot; value=\&quot;\&quot; required onchange=\&quot;updateMarker()\&quot;&gt; \n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;h3&gt;Sensoren&lt;/h3&gt;\n&quot; &quot;  &lt;label for=\&quot;inputRhtsensor\&quot;&gt;Temperatursensor, der benutzt werden soll&lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;select name=\&quot;inputRhtsensor\&quot; id=\&quot;inputRhtsensor\&quot;&gt;\n&quot; &quot;  &lt;option value=\&quot;default\&quot;&gt;(keiner)&lt;/option&gt;\n&quot; &quot;  &lt;option value=\&quot;BME280\&quot;&gt;BME280&lt;/option&gt;\n&quot; &quot;  &lt;option value=\&quot;DHT22\&quot;&gt;DHT22&lt;/option&gt;\n&quot; &quot;  &lt;/select&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;label for=\&quot;inputSds011id\&quot;&gt;ID des SDS011, Feinstaub-Sensors (XXXX-XXXX)&lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;input type=\&quot;text\&quot;  name=\&quot;inputSds011id\&quot; id=\&quot;inputSds011id\&quot; maxlength=\&quot;10\&quot; placeholder=\&quot;XXXX-XXXX\&quot; value=\&quot;\&quot;&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;h3&gt;Datenlizenz&lt;/h3&gt;\n&quot; &quot;  &lt;h4&gt;Lizenz Name&lt;/h4&gt;\n&quot; &quot;  &lt;label for=\&quot;inputLicense\&quot;&gt;Unter der die von deinem Sensor aufgenommenen Daten im SmartAQnet veröffentlicht werden sollen&lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;select name=\&quot;inputLicense\&quot; id=\&quot;inputLicense\&quot; onchange=\&quot;set_required()\&quot;&gt;\n&quot; &quot;  &lt;option value=\&quot;CC0 1.0\&quot;&gt;CC0 1.0 Universell&lt;/option&gt;\n&quot; &quot;  &lt;option value=\&quot;CC BY 4.0\&quot;&gt;CC BY 4.0 Namensnennung International&lt;/option&gt;\n&quot; &quot;  &lt;/select&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;span style=\&quot;font-size: 75%\&quot;&gt; Weitere Informationen: &lt;a href=\&quot;&quot; data1_url_cc0 &quot;\&quot; id=\&quot;license_url\&quot;&gt;&quot; data1_url_cc0 &quot;&lt;/a&gt;&lt;/span&gt;\n&quot; &quot;  &lt;script&gt;\n&quot; &quot;  function set_required() {\n&quot; &quot;    var lic = document.getElementById(&apos;inputLicense&apos;).value;\n&quot; &quot;      if (lic == \&quot;CC BY 4.0\&quot;) {\n&quot; &quot;        document.getElementById(\&quot;inputOwner\&quot;).required = true;\n&quot; &quot;        document.getElementById(\&quot;license_url\&quot;).innerHTML = \&quot;&quot; data1_url_ccby &quot;\&quot;;\n&quot; &quot;        document.getElementById(\&quot;license_url\&quot;).href = \&quot;&quot; data1_url_ccby &quot;\&quot;;\n&quot; &quot;      } else {\n&quot; &quot;        document.getElementById(\&quot;inputOwner\&quot;).required = false;\n&quot; &quot;        document.getElementById(\&quot;license_url\&quot;).innerHTML = \&quot;&quot; data1_url_cc0 &quot;\&quot;;\n&quot; &quot;        document.getElementById(\&quot;license_url\&quot;).href = \&quot;&quot; data1_url_cc0 &quot;\&quot;;\n&quot; &quot;      }\n&quot; &quot;  }\n&quot; &quot;  &lt;/script&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;h4&gt;Legal Notice&lt;/h4&gt;\n&quot; &quot;  &lt;label for=\&quot;inputOwner\&quot;&gt;Für Namensnennung in Lizenz (optional bei CC0)&lt;/label&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;input type=\&quot;text\&quot; name=\&quot;inputOwner\&quot; id=\&quot;inputOwner\&quot; maxlength=\&quot;40\&quot; placeholder=\&quot;Name (optional bei CC0)\&quot; value=\&quot;\&quot;&gt;\n&quot; &quot;  &lt;br /&gt;\n&quot; &quot;  &lt;input type=\&quot;button\&quot; value=\&quot;Speichern\&quot; name=\&quot;cfgButton\&quot; onclick = \&quot;if (confirm(&apos;Daten speichern?&apos;)) _sa(&apos;/config&apos;)\&quot; method=\&quot;post\&quot;&gt;\n&quot; &quot;&lt;script&gt;\n&quot; &quot;function ConfirmBox1() {\n&quot; &quot;  if (confirm(&apos;Daten speichern1?&apos;)) window.location.href = \&quot;/config\&quot;;\n&quot; &quot;}\n&quot; &quot;&lt;/script&gt;\n&quot; &quot;&lt;br /&gt;\n&quot;):&#160;main.h'],['../main_8h.html#a7d03f66dd7454f55ad3fc0524916c93d',1,'ACElement(cfgBody2, &quot;&lt;p&gt;Hinweise: &lt;br /&gt;\n&quot; &quot;&lt;ul&gt;\n&quot; &quot;  &lt;li&gt;Die Daten werden nur gespeichert, wenn sie den \&quot;Speichern\&quot; Button klicken.&lt;/li&gt;\n&quot; &quot;  &lt;li&gt;Die Eingabe neuer Daten überschreibt die alten Daten (z.B. für Standortwechsel).&lt;/li&gt;\n&quot; &quot;  &lt;li&gt;Alle von Ihnen eingegeben Informationen werden offen in der Datenbank abrufbar sein.&lt;/li&gt;\n&quot; &quot;&lt;/ul&gt;\n&quot; &quot;&lt;/p&gt;\n&quot; &quot;&lt;script&gt;\n&quot; &quot;  window.addEventListener(&apos;load&apos;, function(){\n&quot; &quot;    document.getElementById(\&quot;_aux\&quot;).onkeydown = \&quot;return unfocusForm()\&quot;\n&quot; &quot;  });\n&quot; &quot;&lt;/script&gt;\n&quot; &quot;&quot;):&#160;main.h'],['../main_8h.html#a3d955aca8968bd4e1ad980d0df15e44e',1,'ACElement(otaScript, otaUpdate):&#160;main.h']]],
  ['actext',['ACText',['../main_8h.html#a7cd09b55d8faffa1ece9b52c9a294f43',1,'ACText(cfgData, &quot; &quot;):&#160;main.h'],['../main_8h.html#a72c1f96215fbb25f037c234dbd51365e',1,'ACText(cfgtitle, &quot;&quot;, &quot;text-align:center;color:#2f4f4f;&quot;):&#160;main.h']]],
  ['apmode_5freboot_5ftimeout',['APMODE_REBOOT_TIMEOUT',['../main_8h.html#a8a1756fdd61bca61d188ce01ece876b9',1,'main.h']]],
  ['atm',['atm',['../main_8h.html#a7d031eabfc5343396f4e4a58c99f8230',1,'main.h']]]
];
