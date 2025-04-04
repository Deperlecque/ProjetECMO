// --- Simulation de données ECG (PQRST) ---
        // Un cycle ECG simplifié (P, Q, R, S, T) comme une série de points
        // Ces valeurs sont arbitraires et visent à ressembler à un ECG

        let BPM = 60 // battement par minutes
        let TempDunBattement = 570
        let Espacement = 60000/BPM/10 // Espacement entre chaque battement en ms

        const pqrstCycle = [
          0.05, 0.07, 0.1, 0.12, 0.12, 0.1, 0.07, 0.05, // P wave                   *8
          Array(7).fill(0,),        // Segment PR                                   *7
          -0.07, -0.12,              // Q wave                                       *2
          0, 0.98, 0, -0.05,        // R wave (pic)                                 *4
          -0.3,                     // S wave                                       *1
          Array(13).fill(0,),       // Segment ST                                   *13
          0, 0.05, 0.1, 0.1, 0.125, 0.15, 0.15, 0.175, 0.2, 0.2, 0.2, 0.2, 0.175, 0.15, 0.15, 0.125, 0.1,
          0.1, 0.05, 0,       // T wave                                       *20
          0.02, 0.02,               // U wave                                       *2
          ...Array(Espacement).fill(0,) /* Espacement est calculer en prenant le nombre de ms 
                                           entre chaque battement, en divise ensuite par 10  */      
          ];
      let cycleIndex = 0;

      // Fonction pour obtenir la prochaine valeur du cycle ECG
      function getNextEcgValue() {
          // Ajoute un léger bruit aléatoire pour plus de réalisme
          const noise = (Math.random() - 0.5) * 0.025;
          const value = pqrstCycle[cycleIndex] + noise;
          cycleIndex = (cycleIndex + 1) % pqrstCycle.length; // Passe au point suivant et boucle
          return value;
      }

      // --- Configuration de Smoothie Chart ---
      const smoothie = new SmoothieChart({
          grid: {
              strokeStyle: 'rgb(0, 128, 0)', // Couleur de la grille (vert classique)
              fillStyle: 'rgb(0, 0, 0)',     // Couleur de fond (noir)
              lineWidth: 1,
              millisPerLine: 500,            // Lignes verticales toutes les 500ms
              verticalSections: 8,           // Nombre de sections verticales
          },
          labels: {
              fillStyle: 'rgb(0, 255, 0)',   // Couleur des labels (vert clair)
              fontSize: 12,
              precision: 2
          },
          maxValue: 1.1,                     // Valeur max de l'axe Y
          minValue: -0.5,                    // Valeur min de l'axe Y
          responsive: true,                  // S'adapte à la taille du canvas
          millisPerPixel: 7                  // Vitesse de défilement (plus bas = plus rapide)
      });

      // Créer la série temporelle pour les données ECG
      const timeSeries = new TimeSeries();

      // Ajouter la série temporelle au graphique
      smoothie.addTimeSeries(timeSeries, {
          strokeStyle: 'rgb(0, 255, 0)', // Couleur de la ligne ECG (vert clair)
          lineWidth: 2
      });

      // Lier le graphique au canvas HTML
      const canvas = document.getElementById('ecgChart');
      smoothie.streamTo(canvas, 50); // Délai de rafraîchissement (ms) - plus bas = plus fluide

      // --- Démarrage de la simulation ---
      // Ajouter de nouvelles données à intervalles réguliers
      setInterval(function() {
          const timestamp = new Date().getTime();
          const value = getNextEcgValue();
          timeSeries.append(timestamp, value); // Ajoute le nouveau point
      }, 10); // Ajoute une nouvelle donnée toutes les 50ms