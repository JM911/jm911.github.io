---
title: "Git"
layout: archive
permalink: categories/git
author_profile: true
sidebar_main: true
---


{% assign posts = site.categories.['git'] %}
{% for post in posts %} {% include archive-single-category.html type=page.entries_layout %} {% endfor %}