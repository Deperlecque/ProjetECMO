// --- Simulation de données ECG (PQRST) ---
        // Un cycle ECG simplifié (P, Q, R, S, T) comme une série de points
        // Ces valeurs sont arbitraires et visent à ressembler à un ECG
        const pqrstCycle = [
          0, 0, 0, 0, 0, // Ligne de base
          0.1, 0.3, 0.1, // P wave
          0, 0, 0,       // Segment PR
          -0.2,          // Q wave
          1.0,           // R wave (pic)
          -0.4,          // S wave
          0, 0, 0, 0,    // Segment ST
          0.2, 0.4, 0.3, 0.1, // T wave
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // Ligne de base (plus longue pour simuler la fréquence cardiaque)
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          
      ];
      let cycleIndex = 0;

      // Fonction pour obtenir la prochaine valeur du cycle ECG
      function getNextEcgValue() {
          // Ajoute un léger bruit aléatoire pour plus de réalisme
          const noise = (Math.random() - 0.5) * 0.05;
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
      }, 25); // Ajoute une nouvelle donnée toutes les 50ms